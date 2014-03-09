#include "fix_application_impl.h"

#include <quickfix/FileLog.h>
#include <quickfix/FileStore.h>
#include <quickfix/ThreadedSocketInitiator.h>
#include <quickfix/fix42/ExecutionReport.h>
#include <quickfix/fix42/MarketDataIncrementalRefresh.h>
#include <quickfix/fix42/MarketDataRequest.h>
#include <quickfix/fix42/MarketDataRequestReject.h>
#include <quickfix/fix42/MarketDataSnapshotFullRefresh.h>
#include <quickfix/fix42/NewOrderSingle.h>

FIXApplicationImpl::FIXApplicationImpl(Strategy* strategy)
    : order_id_(0),
      strategy_(strategy) {}

FIXApplicationImpl::~FIXApplicationImpl() {
  fprintf(stdout, "Shutting down...\n");

  if (socket_initiator_)
    socket_initiator_->stop();

  delete socket_initiator_;
  delete log_factory_;
  delete message_store_factory_;
  delete session_settings_;
}

void FIXApplicationImpl::Init(const std::string& config_path) {
  session_settings_ = new FIX::SessionSettings(config_path);
  message_store_factory_ = new FIX::FileStoreFactory(*session_settings_);
  log_factory_ = new FIX::FileLogFactory(*session_settings_);
  socket_initiator_ = new FIX::ThreadedSocketInitiator(
      *this, *message_store_factory_, *session_settings_, *log_factory_);
  socket_initiator_->start();
}

bool FIXApplicationImpl::IsLogged() const {
  FIX::Session* market_data_session =
      FIX::Session::lookupSession(market_data_session_id_);
  FIX::Session* order_session = FIX::Session::lookupSession(order_session_id_);
  if (market_data_session && order_session) {
    if (market_data_session->isLoggedOn() && order_session->isLoggedOn()) {
      return true;
    }
    return false;
  }
  return false;
}

void FIXApplicationImpl::RequestMarketData(const std::string& symbol) {
  FIX42::MarketDataRequest msg;
  msg.setField(FIX::Symbol(symbol));
  FIX::Session::sendToTarget(msg, market_data_session_id_);
}

void FIXApplicationImpl::SendOrder(const std::string& symbol,
                                   Order::Side side,
                                   double quantity,
                                   double limit_price) {
  FIX42::NewOrderSingle new_order(
      GetNextOrderId(),
      FIX::HandlInst_AUTOMATED_EXECUTION_ORDER_PRIVATE_NO_BROKER_INTERVENTION,
      symbol,
      side == Order::SIDE_BUY ? FIX::Side_BUY : FIX::Side_SELL,
      FIX::TransactTime(),
      FIX::OrdType_MARKET);

  new_order.set(FIX::OrdType(FIX::OrdType_LIMIT));
  new_order.set(FIX::OrderQty(quantity));
  new_order.set(FIX::Price(limit_price));

  FIX::Session::sendToTarget(new_order, order_session_id_);
}

void FIXApplicationImpl::onCreate(const FIX::SessionID& sessionId) {}

// Called by QF whenever a Session is successfully logged on.
//
// We need to know which SessionID to use when sending orders vs sending a
// market data subscription.  The onLogon() callback is a good time to
// distinguish between the two Sessions.  We will use a couple of custom config
// file options to help us do that.
void FIXApplicationImpl::onLogon(const FIX::SessionID& sessionId) {
  // Ask QF for the SessionSettings for this Session
  const FIX::Dictionary* settings =
      socket_initiator_->getSessionSettings(sessionId);

  // Grab our custom "MyMarketDataSession" parameter (if it exists) from the
  // SessionSettings
  if (settings->has("MyMarketDataSession") &&
      settings->getBool("MyMarketDataSession")) {
    market_data_session_id_ = sessionId;
    fprintf(stdout,
            "[onLogon] %s (MyMarketDataSession)\n",
            market_data_session_id_.toString().c_str());
  }

  // Grab our custom "MyOrderSession" parameter (if it exists) from the
  // SessionSettings
  if (settings->has("MyOrderSession") && settings->getBool("MyOrderSession")) {
    order_session_id_ = sessionId;
    fprintf(stdout,
            "[onLogon] %s (MyOrderSession)\n",
            order_session_id_.toString().c_str());
  }
}

// Called by QF when a Session is either logged out or suddenly disconnected.
// We can use this to notify our application of a lost connection.
void FIXApplicationImpl::onLogout(const FIX::SessionID& session_id) {
  fprintf(stdout, "[onLogout] %s\n", session_id.toString().c_str());
}

// Called by QF right before an Admin-type message is sent to the counterparty.
//
// Examples of Admin-type messages are Logon, Logout, and Heartbeat.  We never
// send these types of messages ourselves -- QF does so for us automatically.
//
// However, we may need to customize the content of an Admin-type -- e.g., our
// counterparty may require us to specify a username or password.
void FIXApplicationImpl::toAdmin(FIX::Message& message,
                                 const FIX::SessionID& sessionId) {}

//-----------------------------------------------------------------------------
// Called by QF after we call Session::sendToTarget() to send a message, right
// before the message is actually transmitted.
//
// Examples of App-type messages that we might send are NewOrderSingle,
// OrderCancelRequest, and MarketDataRequest.
//
// The FIX Protocol guarantees in-order delivery of all messages.  For example,
// if you temporarily lose your network connection, FIX Protocol ensures that
// any messages that failed to make it to either counterparty will be
// re-transmitted.
//
// This is helpful behaviour when you are, say, receiving an ExecutionReport. It
// is probably NOT helpful behaviour if say, you send a NewOrderSingle which
// gets
// re-transmitted an hour later when the network suddenly comes back up and the
// market has moved significantly!
//
// This is your chance to thwart the automatic resend behaviour if you do not
// want it.
void FIXApplicationImpl::toApp(
    FIX::Message& message,
    const FIX::SessionID& sessionID) throw(FIX::DoNotSend) {}

// Called by QF when an Admin-type message is received from the counterparty.
//
// Examples of Admin-type messages are Logon, Logout, and Heartbeat.
//
// We almost never want or need to do anything in this callback.  QF handles
// these types of messages for us automatically.
void FIXApplicationImpl::fromAdmin(
    const FIX::Message&,
    const FIX::SessionID&) throw(FIX::FieldNotFound,
                                 FIX::IncorrectDataFormat,
                                 FIX::IncorrectTagValue,
                                 FIX::RejectLogon) {}

//-----------------------------------------------------------------------------
// Called by QF when an App-type message is received from the counterparty.
//
// We could just write all of our code for handling FIX messages right here in
// this callback.
//
// However, we would probably end up with a really, really long function if we
// did that.  Instead, we usually just call the QF crack() function here, which
// calls the proper onMessage() callback for whatever MsgType we just received.
void FIXApplicationImpl::fromApp(
    const FIX::Message& message,
    const FIX::SessionID& sessionID) throw(FIX::FieldNotFound,
                                           FIX::IncorrectDataFormat,
                                           FIX::IncorrectTagValue,
                                           FIX::UnsupportedMessageType) {
  crack(message, sessionID);
}

void FIXApplicationImpl::onMessage(const FIX42::ExecutionReport& msg,
                                   const FIX::SessionID&) {}

void FIXApplicationImpl::onMessage(
    const FIX42::MarketDataSnapshotFullRefresh& msg,
    const FIX::SessionID&) {
  FIX::MDEntryType type;
  FIX::MDEntryPx price;
  FIX::MDEntrySize quantity;

  msg.getField(type);
  msg.getField(price);
  msg.getField(quantity);

  if (type.getValue() == FIX::MDEntryType_TRADE) {
    strategy_->DumpTrade(price.getValue(), quantity.getValue());
  }
}

void FIXApplicationImpl::onMessage(
    const FIX42::MarketDataIncrementalRefresh& msg,
    const FIX::SessionID&) {}

void FIXApplicationImpl::onMessage(const FIX42::MarketDataRequestReject& msg,
                                   const FIX::SessionID&) {}

std::string FIXApplicationImpl::GetNextOrderId() {
  FIX::Locker lock(mutex_);
  order_id_++;
  std::stringstream s;
  s << order_id_;
  return s.str();
}

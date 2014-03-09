#ifndef CLIENT_FIX_APPLICATION_IMPL_H_
#define CLIENT_FIX_APPLICATION_IMPL_H_

#include <quickfix/Application.h>
#include <quickfix/MessageCracker.h>
#include <quickfix/Mutex.h>

#include "strategy.h"

namespace FIX {
class FileLogFactory;
class MessageStoreFactory;
class SessionSettings;
class ThreadedSocketInitiator;
}

class Order {
 public:
   enum Side { SIDE_BUY, SIDE_SELL };
};

// A simple interface that allows our Strategy to subscribe to market data and
// send orders.
class FIXApplicationImpl : public FIX::Application, public FIX::MessageCracker {
 public:
  explicit FIXApplicationImpl(Strategy* strategy);
  virtual ~FIXApplicationImpl();

  // Establishes FIX session.
  void Init(const std::string& config_path);

  bool IsLogged() const;

  // Subscribes to market data updates for an instrument.
  void RequestMarketData(const std::string& symbol);

  // Send a limit order.
  void SendOrder(const std::string& symbol,
                 Order::Side side,
                 double quantity,
                 double limit_price);

 private:
  // FIX::Application:
  virtual void onCreate(const FIX::SessionID&);
  virtual void onLogon(const FIX::SessionID&);
  virtual void onLogout(const FIX::SessionID&);
  virtual void toAdmin(FIX::Message&, const FIX::SessionID&);
  virtual void toApp(FIX::Message&,
                     const FIX::SessionID&) throw(FIX::DoNotSend);
  virtual void fromAdmin(const FIX::Message&,
                         const FIX::SessionID&) throw(FIX::FieldNotFound,
                                                      FIX::IncorrectDataFormat,
                                                      FIX::IncorrectTagValue,
                                                      FIX::RejectLogon);
  virtual void fromApp(
      const FIX::Message& message,
      const FIX::SessionID& sessionID) throw(FIX::FieldNotFound,
                                             FIX::IncorrectDataFormat,
                                             FIX::IncorrectTagValue,
                                             FIX::UnsupportedMessageType);

  // FIX::MessageCracker:
  virtual void onMessage(const FIX42::ExecutionReport&, const FIX::SessionID&);
  virtual void onMessage(const FIX42::MarketDataSnapshotFullRefresh&,
                         const FIX::SessionID&);
  virtual void onMessage(const FIX42::MarketDataIncrementalRefresh& msg,
                         const FIX::SessionID&);
  virtual void onMessage(const FIX42::MarketDataRequestReject&,
                         const FIX::SessionID&);

  std::string GetNextOrderId();

  FIX::Mutex mutex_;
  int order_id_;

  Strategy* strategy_;

  FIX::SessionID market_data_session_id_;
  FIX::SessionID order_session_id_;
  FIX::MessageStoreFactory* message_store_factory_;
  FIX::FileLogFactory* log_factory_;
  FIX::SessionSettings* session_settings_;
  FIX::ThreadedSocketInitiator* socket_initiator_;
};

#endif  // CLIENT_FIX_APPLICATION_IMPL_H_

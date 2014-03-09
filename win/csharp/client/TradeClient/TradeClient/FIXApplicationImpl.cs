using System;
using QuickFix;
using System.Collections;

namespace TradeClient
{
    class FIXApplicationImpl : MessageCracker, Application
    {
        SessionSettings session_settings_;
        MessageStoreFactory message_store_factory_;
        FileLogFactory file_log_factory_;
        MessageFactory message_factory_;
        ThreadedSocketInitiator socket_initiator_;
        SessionID market_data_session_id_;
        SessionID order_session_id_;

        public FIXApplicationImpl()
        {
            market_data_session_id_ = new SessionID();
            order_session_id_ = new SessionID();
        }

        public void Init(string config_path)
        {
            session_settings_ = new SessionSettings(config_path);
            message_store_factory_ = new FileStoreFactory(session_settings_);
            file_log_factory_ = new FileLogFactory(session_settings_);
            message_factory_ = new DefaultMessageFactory();
            socket_initiator_ = new ThreadedSocketInitiator(this, message_store_factory_, session_settings_, file_log_factory_, message_factory_);
            socket_initiator_.start();
        }

        public bool IsLogged()
        {
            Session market_data_session = Session.lookupSession(market_data_session_id_);
            Session order_session = Session.lookupSession(order_session_id_);
            if (market_data_session != null && order_session != null) {
                if (market_data_session.isLoggedOn() && order_session.isLoggedOn()) {
                    return true;
                }
                return false;
            }
            return false;
        }

        public void RequestMarketData(string symbol)
        {
            QuickFix42.MarketDataRequest msg = new QuickFix42.MarketDataRequest();
            msg.setField(new Symbol(symbol));
            Session.sendToTarget(msg, market_data_session_id_);
        }

        public void fromAdmin(Message msg, SessionID value)
        {
            //throw new NotImplementedException();
        }

        public void fromApp(Message msg, SessionID value)
        {
            crack(msg, value);
        }

        public void onCreate(SessionID value)
        {
            //throw new NotImplementedException();
        }

        public void onLogon(SessionID session_id)
        {
            Dictionary settings = session_settings_.get(session_id);
            if (settings.has("MyMarketDataSession") && settings.getBool("MyMarketDataSession"))
            {
                market_data_session_id_ = session_id;
                Console.WriteLine("[onLogon] {0}", session_id.ToString());
            }

            if (settings.has("MyOrderSession") && settings.getBool("MyOrderSession"))
            {
                order_session_id_ = session_id;
                Console.WriteLine("[onLogon] {0}", session_id.ToString());
            }
        }

        public void onLogout(SessionID value)
        {
            //throw new NotImplementedException();
        }

        public void toAdmin(Message msg, SessionID value)
        {
            //throw new NotImplementedException();
        }

        public void toApp(Message msg, SessionID value)
        {
            //throw new NotImplementedException();
        }

        public override void onMessage(QuickFix42.MarketDataSnapshotFullRefresh msg, SessionID session_id)
        {
            MDEntryType type = new MDEntryType();
            MDEntryPx price = new MDEntryPx();
            MDEntrySize quantity = new MDEntrySize();

            msg.getField(type);
            msg.getField(price);
            msg.getField(quantity);

            if (type.getValue() == MDEntryType.TRADE) {
                Console.WriteLine("{0} / {1}", price.getValue(), quantity.getValue());
            }
        }
    }
}

using System;
using QuickFix;

namespace TradeClient
{
    class FIXApplicationImpl : MessageCracker, Application
    {
        SessionSettings session_settings_;
        MessageStoreFactory message_store_factory_;
        FileLogFactory file_log_factory_;
        MessageFactory message_factory_;
        ThreadedSocketInitiator socket_initiator_;

        public void Init(string config_path)
        {
            session_settings_ = new SessionSettings(config_path);
            message_store_factory_ = new FileStoreFactory(session_settings_);
            file_log_factory_ = new FileLogFactory(session_settings_);
            message_factory_ = new DefaultMessageFactory();
            socket_initiator_ = new ThreadedSocketInitiator(this, message_store_factory_, session_settings_, file_log_factory_, message_factory_);
            socket_initiator_.start();
        }

        public void fromAdmin(Message msg, SessionID value)
        {
            //throw new NotImplementedException();
        }

        public void fromApp(Message msg, SessionID value)
        {
            throw new NotImplementedException();
        }

        public void onCreate(SessionID value)
        {
            //throw new NotImplementedException();
        }

        public void onLogon(SessionID value)
        {
            Console.WriteLine("[onLogon] {0}", value.ToString());
        }

        public void onLogout(SessionID value)
        {
            throw new NotImplementedException();
        }

        public void toAdmin(Message msg, SessionID value)
        {
            //throw new NotImplementedException();
        }

        public void toApp(Message msg, SessionID value)
        {
            throw new NotImplementedException();
        }
    }
}

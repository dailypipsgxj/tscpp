using System;
using QuickFix;

namespace TradeClient
{
    class FIXApplicationImpl : MessageCracker, Application
    {
        public void fromAdmin(Message msg, SessionID value)
        {
            throw new NotImplementedException();
        }

        public void fromApp(Message msg, SessionID value)
        {
            throw new NotImplementedException();
        }

        public void onCreate(SessionID value)
        {
            throw new NotImplementedException();
        }

        public void onLogon(SessionID value)
        {
            throw new NotImplementedException();
        }

        public void onLogout(SessionID value)
        {
            throw new NotImplementedException();
        }

        public void toAdmin(Message msg, SessionID value)
        {
            throw new NotImplementedException();
        }

        public void toApp(Message msg, SessionID value)
        {
            throw new NotImplementedException();
        }
    }
}

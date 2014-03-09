using System;

namespace TradeClient
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length != 2)
            {
                Console.WriteLine("usage: TradeClient.exe config-file SYMBOL");
                Environment.Exit(-1);
            }

            Console.WriteLine("Press [ENTER] to quit...");

            FIXApplicationImpl fix_app = new FIXApplicationImpl();
            fix_app.Init(args[0]);

            while (true)
            {
                if (fix_app.IsLogged())
                {
                    fix_app.RequestMarketData(args[1]);
                    break;
                }
            }

            Console.ReadLine();
        }
    }
}

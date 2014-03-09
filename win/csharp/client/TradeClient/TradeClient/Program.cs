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

            FIXApplicationImpl fix_app = new FIXApplicationImpl();
            fix_app.Init(args[0]);

            Console.WriteLine("Press [ENTER] to quit...");
            Console.ReadLine();
        }
    }
}

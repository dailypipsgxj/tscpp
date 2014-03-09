using System;

namespace TradeClient
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length != 3)
            {
                Console.WriteLine("usage: TradeClient.exe config-file SYMBOL");
                Environment.Exit(-1);
            }
            Console.WriteLine("Press [ENTER] to quit...");
            Console.ReadLine();
        }
    }
}

#include "strategy.h"

#include <stdio.h>

Strategy::Strategy(const std::string& symbol) : symbol_(symbol), vwap_(3) {}

void Strategy::DumpTrade(double price, double quantity) {
  vwap_.AddTrade(price, quantity);

  printf("%.2f %.0f\n", price, quantity);
}

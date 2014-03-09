#include "strategy.h"

#include <stdio.h>

Strategy::Strategy(const std::string& symbol) : symbol_(symbol) {}

void Strategy::DumpTrade(double price, double qty) {
  printf("%.2f %.0f\n", price, qty);
}

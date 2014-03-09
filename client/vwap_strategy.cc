#include "vwap_strategy.h"

#include <stdio.h>

VwapStrategy::VwapStrategy(const std::string& symbol)
    : symbol_(symbol), vwap_(3) {}

VwapStrategy::~VwapStrategy() {}

void VwapStrategy::DumpTrade(double price, double quantity) {
  vwap_.AddTrade(price, quantity);

  const double current_vwap = vwap_.Calculate();

  printf("VWAP %lu = %.2f\n", vwap_.num_trades(), current_vwap);

  printf("%.2f %.0f\n", price, quantity);

  if (price < (current_vwap - 1)) {
    printf("SELL!!!\n");
  } else if (price > (current_vwap + 1)) {
    printf("BUY!!!\n");
  }
}

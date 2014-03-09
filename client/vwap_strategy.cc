#include "vwap_strategy.h"

#include <stdio.h>

VwapStrategy::VwapStrategy(const std::string& symbol)
    : symbol_(symbol), vwap_(3) {}

VwapStrategy::~VwapStrategy() {}

void VwapStrategy::DumpTrade(double price, double quantity) {
  vwap_.AddTrade(price, quantity);

  const double current_vwap = vwap_.Calculate();

  printf("%.2f/%.0f VWAP %lu = %.2f\n", price, quantity, vwap_.num_trades(),
         current_vwap);

  if (price < (current_vwap - 1)) {
    printf("SELL!!!\n");
  } else if (price > (current_vwap + 1)) {
    printf("BUY!!!\n");
  }
}

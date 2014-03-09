#ifndef CLIENT_VWAP_STRATEGY_H_
#define CLIENT_VWAP_STRATEGY_H_

#include <string>

#include "vwap.h"

class VwapStrategy {
 public:
  explicit VwapStrategy(const std::string& symbol);

  void DumpTrade(double price, double quantity);

 private:
  std::string symbol_;

  Vwap vwap_;
};

#endif  // CLIENT_VWAP_STRATEGY_H_

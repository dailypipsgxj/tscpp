#ifndef CLIENT_STRATEGY_H_
#define CLIENT_STRATEGY_H_

#include <string>

#include "vwap.h"

class Strategy {
 public:
  explicit Strategy(const std::string& symbol);

  void DumpTrade(double price, double quantity);

 private:
  std::string symbol_;

  Vwap vwap_;
};

#endif  // CLIENT_STRATEGY_H_

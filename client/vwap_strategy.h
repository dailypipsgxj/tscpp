#ifndef CLIENT_VWAP_STRATEGY_H_
#define CLIENT_VWAP_STRATEGY_H_

#include <string>

#include "strategy.h"
#include "vwap.h"

class VwapStrategy : public Strategy {
 public:
  explicit VwapStrategy(const std::string& symbol);
  virtual ~VwapStrategy();

 private:
  // Strategy:
  virtual void DumpTrade(double price, double quantity) /*OVERRIDE*/;

  std::string symbol_;

  Vwap vwap_;
};

#endif  // CLIENT_VWAP_STRATEGY_H_

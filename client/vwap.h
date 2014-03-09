#ifndef CLIENT_VWAP_H_
#define CLIENT_VWAP_H_

#include <stddef.h>

#include <list>

#include "trade.h"

// Volume-weighted average price.
// http://en.wikipedia.org/wiki/Volume-weighted_average_price
class Vwap {
 public:
  Vwap(size_t num_trades_per_period);

  void AddTrade(double price, double quantity);

  double Calculate();

  void Reset();

  size_t num_trades() const { return trades_.size(); }

 private:
  size_t num_trades_per_period_;
  std::list<Trade> trades_;
};

#endif  // CLIENT_VWAP_H_

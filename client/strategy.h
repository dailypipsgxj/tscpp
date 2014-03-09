#ifndef CLIENT_STRATEGY_H_
#define CLIENT_STRATEGY_H_

#include <string>

class Strategy {
 public:
  explicit Strategy(const std::string& symbol);

  void DumpTrade(double price, double qty);

 private:
  std::string symbol_;
};

#endif  // CLIENT_STRATEGY_H_

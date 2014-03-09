#ifndef CLIENT_STRATEGY_H_
#define CLIENT_STRATEGY_H_

class Strategy {
 public:
  virtual void DumpTrade(double price, double quantity) = 0;

 protected:
  virtual ~Strategy() {}
};

#endif  // CLIENT_STRATEGY_H_

#ifndef CLIENT_TRADE_H_
#define CLIENT_TRADE_H_

class Trade {
 public:
  Trade(double price, double quantity) : price_(price), quantity_(quantity) {}

  double price() const { return price_; }
  double quantity() const { return quantity_; }

 private:
  double price_;
  double quantity_;
};

#endif  // CLIENT_TRADE_H_

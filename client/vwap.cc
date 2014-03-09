#include "vwap.h"

Vwap::Vwap(size_t num_trades_per_period)
    : num_trades_per_period_(num_trades_per_period) {}

void Vwap::AddTrade(double price, double quantity) {
  trades_.push_back(Trade(price, quantity));

  while (trades_.size() > num_trades_per_period_)
    trades_.pop_front();
}

double Vwap::Calculate() {
  if (trades_.empty())
    return 0.0;

  double total_price = 0.0;
  int total_quantity = 0;

  for (std::list<Trade>::const_iterator i = trades_.begin(); i != trades_.end();
       ++i) {
      total_price += (i->price() * i->quantity());
      total_quantity += static_cast<int>(i->quantity());
  }

  if (total_quantity == 0)
    return 0.0;

  return total_price / static_cast<double>(total_quantity);
}

void Vwap::Reset() {
  trades_.clear();
}

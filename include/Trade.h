
#include <vector>

#include "TradeInfo.h"

class Trade {
private:
    TradeInfo bid_trade_;
    TradeInfo ask_trade_;

public:
    Trade(const TradeInfo& bid_trade, const TradeInfo& ask_trade)
        : bid_trade_{bid_trade}, ask_trade_{ask_trade} {}

    const TradeInfo& get_bid_trade() const { return bid_trade_; }
    const TradeInfo& get_ask_trade() const { return ask_trade_; }
};

using Trades = std::vector<Trade>;
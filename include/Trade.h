
#include <iomanip>
#include <iostream>
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

    void display() const {
        std::cout << "\n=== Trade Executed ===\n"
                  << std::setfill(' ') << std::fixed << std::setprecision(2) << "Price:    $"
                  << std::setw(10) << static_cast<double>(bid_trade_.price_)<< "\n"
                  << "Quantity: " << std::setw(11) << bid_trade_.quantity_ << "\n"
                  << "Value:    $" << std::setw(10)
                  << (static_cast<double>(bid_trade_.quantity_) *
                      static_cast<double>(bid_trade_.price_))
                  << "\n\n"
                  << "Buyer ID:  " << bid_trade_.order_id_ << "\n"
                  << "Seller ID: " << ask_trade_.order_id_ << "\n"
                  << "===================" << std::endl;
    }

    //compact display
    //TODO: what is friend
    friend std::ostream& operator<<(std::ostream& os, const Trade& trade) {
        os << "Trade{"
           << "price=" << trade.bid_trade_.price_ << ",qty=" << trade.bid_trade_.quantity_
           << ",bid=" << trade.bid_trade_.order_id_ << ",ask=" << trade.ask_trade_.order_id_ << "}";
        return os;
    }
};

using Trades = std::vector<Trade>;
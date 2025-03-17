
#pragma once

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
                  << "Quantity: "  << bid_trade_.quantity_ << "\n"
                  << "Buyer ID:  " << bid_trade_.order_id_ << "\n"
                  << "Seller ID: " << ask_trade_.order_id_ << "\n"
                  << "===================" << std::endl;
    }

};

using Trades = std::vector<Trade>;
#pragma once
#include <iostream>

#include "LevelInfo.h"
#include <vector>  
#include <iostream>


using LevelInfos = std::vector<LevelInfo>;
class OrderBookLevelInfos {
private:
    LevelInfos bids_; // Using 'm_' prefix to distinguish member variables
    LevelInfos asks_;

public:
    OrderBookLevelInfos(const LevelInfos& bids, const LevelInfos& asks)
        : bids_{bids}, asks_{asks} {} // Using member initializer list

    const LevelInfos& get_bids() const { return bids_; }
    const LevelInfos& get_asks() const { return asks_; }

    friend std::ostream& operator<<(std::ostream& os, const OrderBookLevelInfos& obli) {
        os << "Bids:\n";
        for (const auto& bid : obli.get_bids()) {
            os << "Price: " << bid.price_ << ", Quantity: " << bid.quantity_ << '\n';
        }
        os << "Asks:\n";
        for (const auto& ask : obli.get_asks()) {
            os << "Price: " << ask.price_ << ", Quantity: " << ask.quantity_ << '\n';
        }
        return os;
    }
};

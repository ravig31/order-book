#include <iostream>
#include <map>
#include <set>
#include <list>
#include <cmath>
#include <format>
#include <stdexcept>

enum class OrderType{
    GoodTillCancel,
    FillAndKill
};

enum class Side{
    Buy,
    Sell
};

// creates aliases for the specified types, e.g. Price will be 32bit singed integers
using Price = std::int32_t;
using Quantity = std::uint32_t;
using OrderId = std::uint64_t;

struct LevelInfo{
    Price price_;
    Quantity quantity_;
};

using LevelInfos = std::vector<LevelInfo>;

class OrderBookLevelInfos {
private:
    LevelInfo bids_;  // Using 'm_' prefix to distinguish member variables
    LevelInfo asks_;

public:
    OrderBookLevelInfos(const LevelInfo& bids, const LevelInfo& asks)
        : bids_{bids}, asks_{asks} {}  // Using member initializer list

    const LevelInfo& getBids() const { return bids_; }
    const LevelInfo& getAsks() const { return asks_; }
};

class Order {
private:
    OrderType order_type_;
    OrderId order_id_;
    Side side_;
    Price price_;
    Quantity initial_quantity_;
    Quantity remaining_quantity_;

public:
    Order(OrderType order_type, OrderId order_id, Side side, Price price, Quantity quantity)
        : order_type_{order_type},
        order_id_{order_id},
        side_{side},
        price_{price},
        initial_quantity_{quantity},
        remaining_quantity_{quantity}
    {}

    OrderId get_order_id() const { return order_id_; }
    Side get_side() const { return side_; }
    Price get_price() const { return price_; }
    OrderType get_order_type() const { return order_type_; }
    Quantity get_intitial_quantity() const { return initial_quantity_; }
    Quantity get_remaining_quantity() const { return remaining_quantity_; }
    Quantity get_filled_quantity() const { return get_intitial_quantity() - get_remaining_quantity(); }

    void fill(Quantity quantity){
        if (quantity > get_remaining_quantity())
            throw std::logic_error(std::format("Order ({}) cannot be filled, quantity exceeds remaining", get_order_id()));
    }
    
};

int main(){
    std::cout << "Hello World!";
    return 0;
}
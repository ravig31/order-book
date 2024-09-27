#include <iostream>
#include <map>
#include <set>
#include <list>
#include <cmath>
#include <format>

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
    Price _price;
    Quantity _quantity;
};

using LevelInfos = std::vector<LevelInfo>;

class OrderBookLevelInfos {
private:
    LevelInfo bids;  // Using 'm_' prefix to distinguish member variables
    LevelInfo asks;

public:
    OrderBookLevelInfos(const LevelInfo& _bids, const LevelInfo& _asks)
        : bids{_bids}, asks{_asks} {}  // Using member initializer list

    const LevelInfo& getBids() const { return bids; }
    const LevelInfo& getAsks() const { return asks; }
};

class Order {
private:
    OrderType order_type;
    OrderId order_id;
    Side side;
    Price price;
    Quantity initial_quantity;
    Quantity remaining_quantity;

public:
    Order(OrderType _order_type, OrderId _order_id, Side _side, Price _price, Quantity _quantity)
        : order_type{_order_type},
        order_id{_order_id},
        side{_side},
        price{_price},
        initial_quantity{_quantity},
        remaining_quantity{_quantity}
    {}

    OrderId get_order_id() const { return order_id; }
    Side get_side() const { return side; }
    Price get_price() const { return price; }
    OrderType get_order_type() const { return order_type; }
    Quantity get_intitial_quantity() const { return initial_quantity; }
    Quantity get_remaining_quantity() const { return remaining_quantity; }
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
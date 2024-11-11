#include <map>

#include "LevelInfos.h"
#include "OrderModify.h"
#include "Trade.h"

class OrderBook {
private:
    struct OrderEntry {
        OrderPointer order_{nullptr};      // pointer points to nothing initially
        OrderPointers::iterator location_; //'::iterator' to indicate the type
                                           // is an iterator object
    };

    std::map<Price, OrderPointers, std::greater<Price>> bids_;
    std::map<Price, OrderPointers, std::less<Price>> asks_;
    std::unordered_map<OrderId, OrderEntry> orders_;

    bool can_match(Side side, Price price) const;

    Trades match_orders();

public:
    Trades add_order(OrderPointer order);

    void cancel_order(OrderId order_id);

    Trades match_order(OrderModify order);

    std::size_t size() const;

    OrderBookLevelInfos get_order_infos() const;
};

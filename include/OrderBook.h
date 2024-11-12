#pragma once

#include <condition_variable>
#include <map>
#include <mutex>
#include <thread>

#include "Aliases.h"
#include "LevelInfos.h"
#include "Order.h"
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
    mutable std::mutex orders_mutex_;
    std::thread orders_prune_thread;
    std::condition_variable shutdown;

    void cancel_orders(OrderIds order_id);
    void cancel_order_internal(OrderId order_id);
    void on_order_cancelled(OrderPointer order);
    void remove_expired_good_for_day_orders();
    bool can_match(Side side, Price price) const;
    Trades match_orders();

public:
    Trades add_order(OrderPointer order);
    void cancel_order(OrderId order_id);
    Trades match_order(OrderModify order);
    std::size_t size() const;
    OrderBookLevelInfos get_order_infos() const;
};

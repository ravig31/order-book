#include <map>
#include <numeric>

#include "OrderBook.h"

bool OrderBook::can_match(Side side, Price price) const {
    if (side == Side::Buy) {
        if (asks_.empty())
            return false;

        const auto& [best_ask, _] = *asks_.begin();
        return price >= best_ask;
    } else {
        if (bids_.empty())
            return false;

        const auto& [best_bid, _] = *bids_.begin();
        return best_bid >= price;
    }
}

Trades OrderBook::match_orders() {
    Trades trades;
    trades.reserve(orders_.size());

    while (true) {
        if (bids_.empty() || asks_.empty())
            break;

        auto& [bid_price, bids] = *bids_.begin();
        auto& [ask_price, asks] = *asks_.begin();

        if (bid_price < ask_price)
            break;

        while (bids.size(), asks.size()) {
            auto bid = bids.front();
            auto ask = asks.front();

            // fills the lowest available volume of the both
            Quantity quantity =
                std::min(bid->get_remaining_quantity(), ask->get_remaining_quantity());

            bid->fill(quantity);
            ask->fill(quantity);

            if (bid->is_filled()) {
                bids.pop_front();
                orders_.erase(bid->get_order_id());
            }

            if (ask->is_filled()) {
                asks.pop_front();
                orders_.erase(ask->get_order_id());
            }

            if (bids.empty())
                bids_.erase(bid_price);

            if (asks.empty())
                asks_.erase(ask_price);

            trades.push_back(Trade{TradeInfo{bid->get_order_id(), bid->get_price(), quantity},
                                   TradeInfo{ask->get_order_id(), ask->get_price(), quantity}});
        }
    }
    /*fill and kill aspect, at this point FAK order has been matched hence
    remaining quantity is killed*/
    if (!bids_.empty()) {
        auto& [_, bids] = *bids_.begin();
        auto& order = bids.front();
        if (order->get_order_type() == OrderType::FillAndKill)
            cancel_order(order->get_order_id());
    }
    if (!asks_.empty()) {
        auto& [_, asks] = *asks_.begin();
        auto& order = asks.front();
        if (order->get_order_type() == OrderType::FillAndKill)
            cancel_order(order->get_order_id());
    }
    for (Trade trade : trades) {
        trade.display();
    }
    return trades;
}

Trades OrderBook::add_order(OrderPointer order) {
    if (orders_.contains(order->get_order_id()))
        return {};

    if (order->get_order_type() == OrderType::FillAndKill &&
        !can_match(order->get_side(), order->get_price()))
        return {};

    if (order->get_order_type() == OrderType::Market) {
        if (order->get_side() == Side::Buy && !asks_.empty()) {
            const auto& [worst_ask, _] = *asks_.rbegin();
            order->to_good_till_cancel(worst_ask);
        } else if (order->get_side() == Side::Sell && !bids_.empty()) {
            const auto& [worst_bid, _] = *bids_.rbegin();
            order->to_good_till_cancel(worst_bid);
        } else
            return {};
    };

    OrderPointers::iterator it;
    // add order to bid or ask map
    if (order->get_side() == Side::Buy) {
        // c++ map [] operator automatically creates new entry for price if
        // key does not exist
        auto& orders_at_price = bids_[order->get_price()];
        orders_at_price.push_back(order);
        it = std::prev(orders_at_price.end());
    } else {
        auto& orders_at_price = asks_[order->get_price()];
        orders_at_price.push_back(order);
        it = std::prev(orders_at_price.end());
    }

    // insert new entry for price in orders (insert method already checks if
    // price key exists)
    orders_.insert({order->get_order_id(), OrderEntry{order, it}});
    return match_orders();
}

void OrderBook::cancel_order(OrderId order_id) {
    if (!orders_.contains(order_id))
        return;

    const auto& [order, order_it] = orders_.at(order_id);

    if (order->get_side() == Side::Sell) {
        auto price = order->get_price();
        auto& orders = asks_.at(price);
        orders.erase(order_it);
        if (orders.empty())
            asks_.erase(price);
    } else {
        auto price = order->get_price();
        auto& orders = bids_.at(price);
        orders.erase(order_it);
        if (orders.empty())
            bids_.erase(price);
    }
    orders_.erase(order_id);
}

Trades OrderBook::match_order(OrderModify order) {
    if (!orders_.contains(order.get_order_id()))
        return {};

    const auto& [existing_order, _] = orders_.at(order.get_order_id());
    cancel_order(order.get_order_id());
    return add_order(order.to_order_pointer(existing_order->get_order_type()));
}

std::size_t OrderBook::size() const { return orders_.size(); }

OrderBookLevelInfos OrderBook::get_order_infos() const {
    LevelInfos bid_infos, ask_infos;
    bid_infos.reserve(orders_.size());
    ask_infos.reserve(orders_.size());

    auto create_level_infos = [](Price price, const OrderPointers& orders) -> LevelInfo {
        return LevelInfo{price,
                         std::accumulate(orders.begin(), orders.end(), (Quantity)0,
                                         [](Quantity running_sum, const OrderPointer& order) {
                                             return running_sum + order->get_remaining_quantity();
                                         })};
    };

    for (const auto& [price, orders] : bids_)
        bid_infos.push_back(create_level_infos(price, orders));

    for (const auto& [price, orders] : asks_)
        ask_infos.push_back(create_level_infos(price, orders));

    return OrderBookLevelInfos{bid_infos, ask_infos};
};

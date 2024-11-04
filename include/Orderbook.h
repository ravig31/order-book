#include <map>
#include <numeric>

#include "LevelInfos.h"
#include "OrderModify.h"
#include "Trade.h"

class OrderBook {
 private:
  struct OrderEntry {
    OrderPointer order_{nullptr};  // pointer points to nothing initially
    OrderPointers::iterator
        location_;  //'::iterator' to indicate the type is an iterator object
  };

  std::map<Price, OrderPointers, std::greater<Price>> bids_;
  std::map<Price, OrderPointers, std::less<Price>> asks_;
  std::unordered_map<OrderId, OrderEntry> orders_;

  bool can_match(Side side, Price price) const {
    if (side == Side::Buy) {
      if (asks_.empty()) return false;

      const auto &[best_ask, _] = *asks_.begin();
      return price >= best_ask;
    } else {
      if (bids_.empty()) return false;

      const auto &[best_bid, _] = *bids_.begin();
      return price <= best_bid;
    }
  }

  Trades match_orders() {
    Trades trades;
    trades.reserve(orders_.size());

    while (true) {
      if (bids_.empty() || asks_.empty()) break;

      auto &[bid_price, bids] = *bids_.begin();
      auto &[ask_price, asks] = *asks_.begin();

      if (bid_price < ask_price) break;

      while (bids.size(), asks.size()) {
        auto &bid = bids.front();
        auto &ask = asks.front();

        // fills the lowest available volume of the both
        Quantity quantity = std::min(bid->get_remaining_quantity(),
                                     ask->get_remaining_quantity());

        bid->fill(quantity);
        ask->fill(quantity);

        if (bid->is_filled()) {
          bids.pop_front();
          orders_.erase(bid->get_order_id());
        }

        if (ask->is_filled()) {
          asks.pop_front();
          orders_.erase(bid->get_order_id());
        }

        if (bids.empty()) bids_.erase(bid_price);

        if (asks.empty()) asks_.erase(ask_price);

        trades.push_back(
            Trade{TradeInfo{bid->get_order_id(), bid->get_price(), quantity},
                  TradeInfo{ask->get_order_id(), ask->get_price(), quantity}});
      }
    }
    // fill or kill aspect, checks order book to see if order is still
    // remaining, otherwise kills
    if (!bids_.empty()) {
      auto &[_, bids] = *bids_.begin();
      auto &order = bids.front();
      if (order->get_order_type() == OrderType::FillAndKill)
        cancel_order(order->get_order_id());
    }
    if (!asks_.empty()) {
      auto &[_, asks] = *bids_.begin();
      auto &order = asks.front();
      if (order->get_order_type() == OrderType::FillAndKill)
        cancel_order(order->get_order_id());
    }

    return trades;
  }

 public:
  Trades add_order(OrderPointer order) {
    if (orders_.contains(order->get_order_id())) return {};

    if (order->get_order_type() == OrderType::FillAndKill &&
        !can_match(order->get_side(), order->get_price()))
      return {};

    OrderPointers::iterator it;

    if (order->get_side() == Side::Buy) {
      auto &orders = bids_[order->get_price()];
      orders.push_back(order);
      it = std::next(orders.begin(), orders.size() - 1);
    } else {
      auto &orders = asks_[order->get_price()];
      orders.push_back(order);
      it = std::next(orders.begin(), orders.size() - 1);
    }

    orders_.insert({order->get_order_id(), OrderEntry{order, it}});
    return match_orders();
  }

  void cancel_order(OrderId order_id) {
    if (!orders_.contains(order_id)) return;

    const auto &[order, order_it] = orders_.at(order_id);

    if (order->get_side() == Side::Sell) {
      auto price = order->get_price();
      auto &orders = asks_.at(price);
      orders.erase(order_it);
      if (orders.empty()) asks_.erase(price);
    } else {
      auto price = order->get_price();
      auto &orders = bids_.at(price);
      orders.erase(order_it);
      if (orders.empty()) bids_.erase(price);
    }
    orders_.erase(order_id);
  }

  Trades match_order(OrderModify order) {
    if (!orders_.contains(order.get_order_id())) return {};

    const auto &[existing_order, _] = orders_.at(order.get_order_id());
    cancel_order(order.get_order_id());
    return add_order(order.to_order_pointer(existing_order->get_order_type()));
  }

  std::size_t size() const { return orders_.size(); }

  OrderBookLevelInfos get_order_infos() const {
    LevelInfos bid_infos, ask_infos;
    bid_infos.reserve(orders_.size());
    ask_infos.reserve(orders_.size());

    auto create_level_infos = [](Price price,
                                 const OrderPointers &orders) -> LevelInfo {
      return LevelInfo{
          price, std::accumulate(
                     orders.begin(), orders.end(), (Quantity)0,
                     [](Quantity running_sum, const OrderPointer &order) {
                       return running_sum + order->get_remaining_quantity();
                     })};
    };

    for (const auto &[price, orders] : bids_)
      bid_infos.push_back(create_level_infos(price, orders));

    for (const auto &[price, orders] : asks_)
      ask_infos.push_back(create_level_infos(price, orders));

    return OrderBookLevelInfos{bid_infos, ask_infos};
  }
};

#include <cstddef>
#include <fmt/base.h>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <format>
// #include <cmath>
// #include <stdexcept>
#include <map>
#include <memory>
#include <numeric>
#include <unordered_map>
#include <vector>

enum class OrderType { GoodTillCancel, FillAndKill };

enum class Side { Buy, Sell };

// creates aliases for the specified types, e.g. Price will be 32bit singed
// integers
using Price = std::int32_t;
using Quantity = std::uint32_t;
using OrderId = std::uint64_t;

struct LevelInfo { 
  Price price_;
  Quantity quantity_;
};

using LevelInfos = std::vector<LevelInfo>;

class OrderBookLevelInfos {
private:
  LevelInfos bids_; // Using 'm_' prefix to distinguish member variables
  LevelInfos asks_;

public:
  OrderBookLevelInfos(const LevelInfos& bids, const LevelInfos& asks)
	  : bids_{bids}, asks_{asks} {} // Using member initializer list

  const LevelInfos &get_bids() const { return bids_; }
  const LevelInfos &get_asks() const { return asks_; }

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

class Order {
private:
  OrderType order_type_;
  OrderId order_id_;
  Side side_;
  Price price_;
  Quantity initial_quantity_;
  Quantity remaining_quantity_;

public:
  Order(OrderType order_type, OrderId order_id, Side side, Price price,
		Quantity quantity)
	  : order_type_{order_type}, order_id_{order_id}, side_{side},
		price_{price}, initial_quantity_{quantity},
		remaining_quantity_{quantity} {}

  OrderId get_order_id() const { return order_id_; }
  Side get_side() const { return side_; }
  Price get_price() const { return price_; }
  OrderType get_order_type() const { return order_type_; }
  Quantity get_intitial_quantity() const { return initial_quantity_; }
  Quantity get_remaining_quantity() const { return remaining_quantity_; }
  Quantity get_filled_quantity() const {
	return get_intitial_quantity() - get_remaining_quantity();
  }
  bool is_filled() const { return get_remaining_quantity() == 0; }

  void fill(Quantity quantity) {
	if (quantity > get_remaining_quantity())
	  throw std::logic_error("Order (" + std::to_string(get_order_id()) +
							 ") cannot be filled, quantity exceeds remaining");
	std::string error_message =
		std::format("Order ({}) cannot be filled, quantity exceeds remaining",
					get_order_id());
	throw std::logic_error(error_message);
	remaining_quantity_ -= quantity;
  }
};

// Order can be stored in orders dict and bid/ask dict
using OrderPointer = std::shared_ptr<Order>;
using OrderPointers = std::list<OrderPointer>;

class OrderModify {

private:
  OrderId order_id_;
  Side side_;
  Price price_;
  Quantity quantity_;

public:
  OrderModify(OrderId orderId, Side side, Price price, Quantity quantity)
	  : order_id_{orderId}, side_{side}, price_{price}, quantity_{quantity} {}

  OrderId get_order_id() const { return order_id_; }
  Side get_side() const { return side_; }
  Price get_price() const { return price_; }
  Quantity get_quantity() const { return quantity_; }

  OrderPointer to_order_pointer(OrderType type) const {
	return std::make_shared<Order>(type, get_order_id(), get_side(), get_price(), get_quantity());
  }
};

struct TradeInfo {
  OrderId order_id_;
  Price price_;
  Quantity quantity_;
};

class Trade {

private:
  TradeInfo bid_trade_;
  TradeInfo ask_trade_;

public:
  Trade(const TradeInfo &bid_trade, const TradeInfo &ask_trade)
	  : bid_trade_{bid_trade}, ask_trade_{ask_trade} {}

  const TradeInfo &get_bid_trade() const { return bid_trade_; }
  const TradeInfo &get_ask_trade() const { return ask_trade_; }
};

using Trades = std::vector<Trade>;

class OrderBook {
private:
  struct OrderEntry {
	OrderPointer order_{nullptr}; // pointer points to nothing initially
	OrderPointers::iterator
		location_; //'::iterator' to indicate the type is an iterator object
  };

  std::map<Price, OrderPointers, std::greater<Price>> bids_;
  std::map<Price, OrderPointers, std::less<Price>> asks_;
  std::unordered_map<OrderId, OrderEntry> orders_;

  bool can_match(Side side, Price price) const {
	if (side == Side::Buy) {
	  if (asks_.empty())
		return false;

	  const auto &[best_ask, _] = *asks_.begin();
	  return price >= best_ask;
	} else {
	  if (bids_.empty())
		return false;

	  const auto &[best_bid, _] = *bids_.begin();
	  return price <= best_bid;
	}
  }

  Trades match_orders() {
	Trades trades;
	trades.reserve(orders_.size());

	while (true) {
	  if (bids_.empty() || asks_.empty())
		break;

	  auto &[bid_price, bids] = *bids_.begin();
	  auto &[ask_price, asks] = *asks_.begin();

	  if (bid_price < ask_price)
		break;

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

		if (bids.empty())
		  bids_.erase(bid_price);

		if (asks.empty())
		  asks_.erase(ask_price);

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
	if (orders_.contains(order->get_order_id()))
	  return {};

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
	if (!orders_.contains(order_id))
	  return;

	const auto &[order, order_it] = orders_.at(order_id);

	if (order->get_side() == Side::Sell) {
	  auto price = order->get_price();
	  auto &orders = asks_.at(price);
	  orders.erase(order_it);
	  if (orders.empty())
		asks_.erase(price);
	} else {
	  auto price = order->get_price();
	  auto &orders = bids_.at(price);
	  orders.erase(order_it);
	  if (orders.empty())
		bids_.erase(price);
	}
	orders_.erase(order_id);
  }

  Trades match_order(OrderModify order) {
	if (!orders_.contains(order.get_order_id()))
	  return {};

	const auto &[existing_order, _] = orders_.at(order.get_order_id());
	cancel_order(order.get_order_id());
	return add_order(order.to_order_pointer(existing_order->get_order_type()));
  }

  std::size_t size() const { return orders_.size(); }

  OrderBookLevelInfos get_order_infos() const {
	LevelInfos bid_infos, ask_infos;
	bid_infos.reserve(orders_.size());
	ask_infos.reserve(orders_.size());

	auto create_level_infos = [](Price price , const OrderPointers& orders) -> LevelInfo {
		return LevelInfo{ price, std::accumulate(orders.begin(), orders.end(), (Quantity)0,
			[](Quantity running_sum, const OrderPointer& order)
			{return running_sum + order->get_remaining_quantity(); }) };
	};

	for (const auto& [price, orders] : bids_)
		bid_infos.push_back(create_level_infos(price, orders));

	for (const auto& [price, orders] : asks_)
		ask_infos.push_back(create_level_infos(price, orders));
	
	return OrderBookLevelInfos{bid_infos, ask_infos};
  }


};

int main() {
	OrderBook ob;
	const OrderId id1 = 1;
	const OrderId id2 = 2;
	ob.add_order(std::make_shared<Order>(OrderType::GoodTillCancel, id1, Side::Buy, 100, 10));
	ob.add_order(std::make_shared<Order>(OrderType::GoodTillCancel, id2, Side::Sell,120, 10));
	std::cout << ob.get_order_infos();
	// std::cout<< ob.size() << std::endl;
	// ob.cancel_order(id);
	// std:: cout << ob.size() << std::endl;
  	return 0;
}	
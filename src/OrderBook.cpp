#include <chrono>
#include <ctime>
#include <numeric>

#include "Aliases.h"
#include "LevelInfo.h"
#include "Order.h"
#include "OrderBook.h"
#include "OrderType.h"
#include "Side.h"

void OrderBook::remove_expired_good_for_day_orders()
{
	using namespace std::chrono;
	const auto end_of_trading_day{ hours(16) };

	while (true)
	{
		const auto now = system_clock::now();
		const auto time_now = system_clock::to_time_t(now);
		std::tm tm_now;
		localtime_r(&time_now, &tm_now);

		if (tm_now.tm_hour >= end_of_trading_day.count())
		{
			tm_now.tm_mday += 1;
		}

		tm_now.tm_hour = end_of_trading_day.count();
		tm_now.tm_min = 0;
		tm_now.tm_sec = 0;

		std::vector<OrderId> orders_to_cancel;

		for (const auto& [id, order_entry] : orders_)
		{
			const auto& [order, _] = order_entry;

			if (order->get_order_type() == OrderType::GoodTillCancel)
				orders_to_cancel.push_back((order->get_order_id()));
		}

		cancel_orders(orders_to_cancel);
	}
}

void OrderBook::cancel_orders(std::vector<OrderId> order_ids)
{
	for (const auto& id : order_ids)
	{
		cancel_order_single(id);
	}
}

void OrderBook::cancel_order_single(OrderId order_id)
{
	if (!orders_.contains(order_id))
		return;

	const auto [order, order_it] = orders_.at(order_id);
	orders_.erase(order_id);

	if (order->get_side() == Side::Sell)
	{
		auto price = order->get_price();
		auto& orders = asks_.at(price);
		orders.erase(order_it);
		if (orders.empty())
			asks_.erase(price);
	}
	else
	{
		auto price = order->get_price();
		auto& orders = bids_.at(price);
		orders.erase(order_it);
		if (orders.empty())
			bids_.erase(price);
	}

	on_order_cancelled(order);
}

void OrderBook::on_order_cancelled(OrderPointer order)
{
	update_level_data(
		order->get_price(),
		order->get_remaining_quantity(),
		LevelData::Action::Remove
	);
}

void OrderBook::on_order_added(OrderPointer order)
{
	update_level_data(order->get_price(), order->get_intitial_quantity(), LevelData::Action::Add);
}

void OrderBook::on_order_matched(Price price, Quantity quantity, bool is_fully_filled)
{
	update_level_data(
		price,
		quantity,
		is_fully_filled ? LevelData::Action::Remove : LevelData::Action::Match
	);
}

void OrderBook::update_level_data(Price price, Quantity quantity, LevelData::Action action)
{
	auto& level_data = levels_[price];
	switch (action)
	{
	case LevelData::Action::Add:
		level_data.order_count_++;
		level_data.quantity_ += quantity;
		break;
	case LevelData::Action::Remove:
	case LevelData::Action::Match:
		level_data.order_count_--;
		level_data.quantity_ -= quantity;
		break;
	}
	if (level_data.order_count_ == 0)
		levels_.erase(price);
}

bool OrderBook::can_match(Side side, Price price) const
{
	if (side == Side::Buy)
	{
		if (asks_.empty())
			return false;

		const auto& [best_ask_price, _] = *asks_.begin();
		return price >= best_ask_price;
	}
	else
	{
		if (bids_.empty())
			return false;

		const auto& [best_bid_price, _] = *bids_.begin();
		return best_bid_price >= price;
	}
}

bool OrderBook::can_fully_fill(Side side, Price price, Quantity quantity)
{
	if (!can_match(side, price))
	{
		return false;
	}

	for (const auto& [level_price, level_data] : levels_)
	{
		if ((side == Side::Buy && level_price > price) ||
			(side == Side::Sell && level_price < price))
			continue;
		if (level_data.quantity_ >= quantity)
			return true;

		quantity -= level_data.quantity_;
	}
	return false;
}

Trades OrderBook::match_orders()
{
	Trades trades;
	trades.reserve(orders_.size());

	while (true)
	{
		if (bids_.empty() || asks_.empty())
			break;

		auto& [best_bid_price, bids_at_level] = *bids_.begin();
		auto& [best_ask_price, asks_at_level] = *asks_.begin();

		if (best_bid_price < best_ask_price)
			break;

		while (bids_at_level.size() && asks_at_level.size())
		{
			auto bid = bids_at_level.front();
			auto ask = asks_at_level.front();

			// fills the lowest available volume of the both
			Quantity quantity =
				std::min(bid->get_remaining_quantity(), ask->get_remaining_quantity());

			bid->fill(quantity);
			ask->fill(quantity);

			if (bid->is_filled())
			{
				bids_at_level.pop_front();
				orders_.erase(bid->get_order_id());
			}

			if (ask->is_filled())
			{
				asks_at_level.pop_front();
				orders_.erase(ask->get_order_id());
			}

			trades.push_back(Trade{ TradeInfo{ bid->get_order_id(), bid->get_price(), quantity },
									TradeInfo{ ask->get_order_id(), ask->get_price(), quantity } });

			on_order_matched(bid->get_price(), quantity, bid->is_filled());
			on_order_matched(ask->get_price(), quantity, ask->is_filled());
		}

		if (bids_at_level.empty())
			bids_.erase(best_bid_price);

		if (asks_at_level.empty())
			asks_.erase(best_ask_price);
	}

	/*fill and kill, if top bid or ask are partially filled kill it*/
	if (!bids_.empty())
	{
		auto& [_, bids_at_level] = *bids_.begin();
		auto& order = bids_at_level.front();
		if (order->get_order_type() == OrderType::FillAndKill)
			cancel_order(order->get_order_id());
	}
	if (!asks_.empty())
	{
		auto& [_, asks_at_level] = *asks_.begin();
		auto& order = asks_at_level.front();
		if (order->get_order_type() == OrderType::FillAndKill)
			cancel_order(order->get_order_id());
	}
	for (Trade trade : trades)
	{
		trade.display();
	}
	return trades;
}

Trades OrderBook::add_order(OrderPointer order)
{
	if (orders_.contains(order->get_order_id()))
		return {};

	if (order->get_order_type() == OrderType::FillAndKill &&
		!can_match(order->get_side(), order->get_price()))
		return {};

	if (order->get_order_type() == OrderType::FillOrKill &&
		!can_fully_fill(order->get_side(), order->get_price(), order->get_intitial_quantity()))
		return {};

	if (order->get_order_type() == OrderType::Market)
	{
		// Convert to GTC with worst ask/bid to guarantee execution
		if (order->get_side() == Side::Buy && !asks_.empty())
		{
			const auto& [worst_ask, _] = *asks_.rbegin();
			order->to_good_till_cancel(worst_ask);
		}
		else if (order->get_side() == Side::Sell && !bids_.empty())
		{
			const auto& [worst_bid, _] = *bids_.rbegin();
			order->to_good_till_cancel(worst_bid);
		}
		else
			return {};
	};

	// add order to bid or ask map
	OrderPointers::iterator it;
	if (order->get_side() == Side::Buy)
	{
		// c++ map [] operator automatically creates new entry for price if key does not exist
		auto& orders_at_price = bids_[order->get_price()];
		orders_at_price.push_back(order);
		it = std::prev(orders_at_price.end());
	}
	else
	{
		auto& orders_at_price = asks_[order->get_price()];
		orders_at_price.push_back(order);
		it = std::prev(orders_at_price.end());
	}

	orders_.insert({ order->get_order_id(), OrderEntry{ order, it } });
	on_order_added(order);
	return match_orders();
}

void OrderBook::cancel_order(OrderId order_id) { cancel_order_single(order_id); }

Trades OrderBook::modify_order(OrderModify order)
{
	if (!orders_.contains(order.get_order_id()))
		return {};

	const auto& [existing_order, _] = orders_.at(order.get_order_id());
	OrderType existing_type = existing_order->get_order_type();
	cancel_order(order.get_order_id());
	return add_order(order.to_order_pointer(existing_type));
}

std::size_t OrderBook::size() const { return orders_.size(); }

OrderBookLevelInfos OrderBook::get_order_infos() const
{
	LevelInfos bid_infos, ask_infos;
	bid_infos.reserve(orders_.size());
	ask_infos.reserve(orders_.size());

	auto create_level_infos = [](Price price, const OrderPointers& orders) -> LevelInfo
	{
		return LevelInfo{ price,
						  std::accumulate(
							  orders.begin(),
							  orders.end(),
							  (Quantity)0,
							  [](Quantity running_sum, const OrderPointer& order)
							  { return running_sum + order->get_remaining_quantity(); }
						  ) };
	};

	for (const auto& [price, orders] : bids_)
		bid_infos.push_back(create_level_infos(price, orders));

	for (const auto& [price, orders] : asks_)
		ask_infos.push_back(create_level_infos(price, orders));

	return OrderBookLevelInfos{ bid_infos, ask_infos };
};

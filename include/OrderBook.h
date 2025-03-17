#pragma once
#include <map>
#include <unordered_map>

#include "Aliases.h"
#include "LevelInfos.h"
#include "Order.h"
#include "OrderModify.h"
#include "Trade.h"

class OrderBook
{
  private:
	struct OrderEntry
	{
		OrderPointer order_{ nullptr };	   // pointer points to nothing initially
		OrderPointers::iterator location_; //'::iterator' to indicate the type
										   // is an iterator object
	};

	struct LevelData
	{
		Quantity quantity_{};
		Quantity order_count_{};

		enum class Action
		{
			Add,
			Remove,
			Match,
		};
	};

	std::unordered_map<Price, LevelData> levels_;
	std::map<Price, OrderPointers, std::greater<Price>> bids_;
	std::map<Price, OrderPointers, std::less<Price>> asks_;
	std::unordered_map<OrderId, OrderEntry> orders_;

	void remove_expired_good_for_day_orders();

	void cancel_orders(std::vector<OrderId> order_id);
	void cancel_order_single(OrderId order_id);

	void on_order_added(OrderPointer order);
	void on_order_matched(Price price, Quantity quantity, bool is_fully_filled);
	void on_order_cancelled(OrderPointer order);
	void update_level_data(Price price, Quantity quantity, LevelData::Action action);

	bool can_fully_fill(Side side, Price price, Quantity quantity);
	bool can_match(Side side, Price price) const;
	Trades match_orders();

  public:
	Trades add_order(OrderPointer order);
	void cancel_order(OrderId order_id);
	Trades modify_order(OrderModify order);
	OrderBookLevelInfos get_order_infos() const;
	std::size_t size() const;
};

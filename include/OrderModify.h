#pragma once

#include "Order.h"

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
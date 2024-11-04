#include <fmt/base.h>

#include <format>
#include <list>
#include "Aliases.h"
#include "OrderType.h"
#include "Side.h"

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
      : order_type_{order_type},
        order_id_{order_id},
        side_{side},
        price_{price},
        initial_quantity_{quantity},
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

using OrderPointer = std::shared_ptr<Order>;
using OrderPointers = std::list<OrderPointer>;


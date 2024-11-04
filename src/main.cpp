#include <iostream>

#include "Orderbook.h"

int main() {
  OrderBook ob;
  const OrderId id1 = 1;
  const OrderId id2 = 2;
  ob.add_order(std::make_shared<Order>(OrderType::GoodTillCancel, id1,
                                       Side::Buy, 100, 10));
  ob.add_order(std::make_shared<Order>(OrderType::GoodTillCancel, id2,
                                       Side::Sell, 120, 10));
  std::cout << ob.get_order_infos();
  // std::cout<< ob.size() << std::endl;
  // ob.cancel_order(id);
  // std:: cout << ob.size() << std::endl;
  return 0;
}
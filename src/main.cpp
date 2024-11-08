#include "OrderBook.h"
#include <iostream>
#include <memory>

int main() {
    OrderBook ob{};
    Trades trades ;
    ob.add_order(
        std::make_shared<Order>(OrderType::GoodTillCancel, 1, Side::Sell, 100, 11));
    ob.add_order(
        std::make_shared<Order>(OrderType::Market, 2, Side::Buy, 100));

    std::cout << ob.get_order_infos();
    return 0;
}
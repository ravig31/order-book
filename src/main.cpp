#include "OrderBook.h"
#include <iostream>
#include <memory>



int main() {
    OrderBook ob{};
    Trades trades;

    // GoodTillCancel Orders
    ob.add_order(
        std::make_shared<Order>(OrderType::GoodTillCancel, 1, Side::Sell, 11, 100));
    ob.add_order(
        std::make_shared<Order>(OrderType::GoodTillCancel, 2, Side::Sell, 11, 110));
    ob.add_order(
        std::make_shared<Order>(OrderType::GoodTillCancel, 4, Side::Buy, 12, 50));
    ob.add_order(
        std::make_shared<Order>(OrderType::GoodTillCancel, 5, Side::Buy, 10, 200));

    // Market Orders
    ob.add_order(
        std::make_shared<Order>(OrderType::Market, 3, Side::Buy, 5));
    ob.add_order(
        std::make_shared<Order>(OrderType::Market, 6, Side::Sell, 10));

    // FillAndKill Orders
    ob.add_order(
        std::make_shared<Order>(OrderType::FillAndKill, 7, Side::Buy, 13, 75));
    ob.add_order(
        std::make_shared<Order>(OrderType::FillAndKill, 8, Side::Sell, 10, 150));

    // FillOrKill Orders
    ob.add_order(
        std::make_shared<Order>(OrderType::FillOrKill, 9, Side::Buy, 14, 60));
    ob.add_order(
        std::make_shared<Order>(OrderType::FillOrKill, 10, Side::Sell, 9, 120));

    // GoodForDay Orders
    ob.add_order(
        std::make_shared<Order>(OrderType::GoodForDay, 11, Side::Buy, 15, 80));
    ob.add_order(
        std::make_shared<Order>(OrderType::GoodForDay, 12, Side::Sell, 8, 130));

    // Cancel Order Cases:
    ob.cancel_order(2); // Cancel order with ID 2
    ob.cancel_order(7); // Cancel FillAndKill order
    ob.cancel_order(15); // Cancel non existent order. Should do nothing.

    // Modify Order Cases:
    OrderModify modify1{4, Side::Buy, 13, 60}; // Modify order 4 price and quantity
    ob.modify_order(modify1);

    OrderModify modify2{12, Side::Sell, 7, 140}; // Modify order 12 price, side, and quantity
    ob.modify_order(modify2);

    OrderModify modify3{20, Side::Buy, 10, 50}; // Modify non existent order. Should do nothing and return empty trades.
    ob.modify_order(modify3);

    std::cout << ob.get_order_infos();
    return 0;
}
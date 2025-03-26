#include "OrderBook.h"
#include <benchmark/benchmark.h>
#include <memory>

static void BM_OrderBookAddOrder(benchmark::State& state) {
    for (auto _ : state) {
        OrderBook ob{};
        ob.add_order(std::make_shared<Order>(OrderType::GoodTillCancel, 1, Side::Sell, 11, 100));
        ob.add_order(std::make_shared<Order>(OrderType::GoodTillCancel, 2, Side::Sell, 11, 110));
        ob.add_order(std::make_shared<Order>(OrderType::GoodTillCancel, 4, Side::Buy, 12, 50));
        ob.add_order(std::make_shared<Order>(OrderType::GoodTillCancel, 5, Side::Buy, 10, 200));
    }
}
BENCHMARK(BM_OrderBookAddOrder);

static void BM_OrderBookProcessOrders(benchmark::State& state) {
    for (auto _ : state) {
        OrderBook ob{};
        ob.add_order(std::make_shared<Order>(OrderType::GoodTillCancel, 1, Side::Sell, 11, 100));
        ob.add_order(std::make_shared<Order>(OrderType::GoodTillCancel, 2, Side::Sell, 11, 110));
        ob.add_order(std::make_shared<Order>(OrderType::GoodTillCancel, 4, Side::Buy, 12, 50));
        ob.add_order(std::make_shared<Order>(OrderType::GoodTillCancel, 5, Side::Buy, 10, 200));
        ob.add_order(std::make_shared<Order>(OrderType::Market, 3, Side::Buy, 5));
        ob.add_order(std::make_shared<Order>(OrderType::Market, 6, Side::Sell, 10));
        ob.add_order(std::make_shared<Order>(OrderType::FillAndKill, 7, Side::Buy, 13, 75));
        ob.add_order(std::make_shared<Order>(OrderType::FillAndKill, 8, Side::Sell, 10, 150));
        ob.add_order(std::make_shared<Order>(OrderType::FillOrKill, 9, Side::Buy, 14, 60));
        ob.add_order(std::make_shared<Order>(OrderType::FillOrKill, 10, Side::Sell, 9, 120));
        ob.add_order(std::make_shared<Order>(OrderType::GoodForDay, 11, Side::Buy, 15, 80));
        ob.add_order(std::make_shared<Order>(OrderType::GoodForDay, 12, Side::Sell, 8, 130));
        ob.cancel_order(2);
        ob.cancel_order(7);
        ob.cancel_order(15);
        OrderModify modify1{4, Side::Buy, 13, 60};
        ob.modify_order(modify1);
        OrderModify modify2{12, Side::Sell, 7, 140};
        ob.modify_order(modify2);
        OrderModify modify3{20, Side::Buy, 10, 50};
        ob.modify_order(modify3);
    }
}
BENCHMARK(BM_OrderBookProcessOrders);

BENCHMARK_MAIN();
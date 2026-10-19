#include <iostream>

#include <lob/order_book.hpp>

int main(){

    lob::OrderRequest orderOne = {50, 100, lob::Side::Buy};
    lob::OrderRequest orderTwo = {52, 200, lob::Side::Sell};
    lob::OrderRequest orderThree = {55, 10, lob::Side::Buy};

    lob::OrderBook orderBook;
    auto ret1 = orderBook.submit(orderOne);
    auto ret2 = orderBook.submit(orderTwo);
    auto ret3 = orderBook.submit(orderThree);

    auto bestBid = orderBook.bestBid();
    auto bestAsk = orderBook.bestAsk();

    if (bestBid.has_value()){
        std::cout << "Best bid price: " << bestBid.value() << "\n";
    }

    if (bestAsk.has_value()){
        std::cout << "Best ask price: " << bestAsk.value() << "\n";
    }

    std::cout << "Quantity for bid price 51 is: " << orderBook.bidQuantityAt(51) << "\n";
    std::cout << "Quantity for ask price 52 is: " << orderBook.askQuantityAt(52) << "\n";

    // Level testing
    lob::LimitOrder testOrder = {10, 30, 100, 100, lob::Side::Buy};
    lob::Level testLevel;
    testLevel.addOrder(testOrder);
    std::cout << "Total Quantity in Level: " << testLevel.getQuantity() << "\n";

    auto fill = testLevel.fillFront(50);
    std::cout << "Fill restingOrderId, price, quantity: " << fill.restingOrderId << ", " << fill.price << ", " << fill.quantity << "\n";

    // Execution results
    std::cout << "ExecutionResult for ret1: orderId, quantity filled, quantity remaining" << ret1.orderId << ", " << ret1.quantityFilled << ", " << ret1.quantityRemaining << "\n";

    std::cout << "ExecutionResult for ret2: orderId, quantity filled, quantity remaining" << ret2.orderId << ", " << ret2.quantityFilled << ", " << ret2.quantityRemaining << "\n";

    std::cout << "ExecutionResult for ret3: orderId, quantity filled, quantity remaining" << ret3.orderId << ", " << ret3.quantityFilled << ", " << ret3.quantityRemaining << "\n";
    return 0;
}


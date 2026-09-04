#include <iostream>

#include <lob/order_book.hpp>

int main(){

    lob::OrderRequest orderOne = {50, 100, lob::Side::Buy};
    lob::OrderRequest orderTwo = {52, 200, lob::Side::Sell};
    lob::OrderRequest orderThree = {51, 10, lob::Side::Buy};

    lob::OrderBook orderBook;
    orderBook.submit(orderOne);
    orderBook.submit(orderTwo);
    orderBook.submit(orderThree);

    std::cout << "Best bid price: " << orderBook.bestBid().value_or(0) << "\n";
    std::cout << "Best ask price: " << orderBook.bestAsk().value_or(0) << "\n";

    std::cout << "Quantity for bid price 51 is: " << orderBook.bidQuantityAt(51) << "\n";
    std::cout << "Quantity for ask price 52 is: " << orderBook.askQuantityAt(52) << "\n";

    return 0;
}


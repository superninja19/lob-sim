#include <cassert>

#include <lob/order_book.hpp>

void testOrderBook_RestingBidsIncomingSell_Filled(){
    lob::OrderBook orderBook;

    lob::OrderRequest order1 = {
        .price = 5024,
        .quantity = 500,
        .side = lob::Side::Buy
    };

    lob::OrderRequest order2 = {
        .price = 5023,
        .quantity = 700,
        .side = lob::Side::Buy
    };

    lob::OrderRequest order3 = {
        .price = 5022,
        .quantity = 1200,
        .side = lob::Side::Buy
    };

    orderBook.submit(order1);
    orderBook.submit(order2);
    orderBook.submit(order3);

    lob::OrderRequest order4 = {
        .price = 5020,
        .quantity = 800,
        .side = lob::Side::Sell
    };

    auto executionResult = orderBook.submit(order4);
    assert(executionResult.status == lob::ExecutionStatus::Filled);
    assert(executionResult.quantityFilled == 800);
    assert(executionResult.quantityRemaining == 0);

    auto trades = orderBook.getTrades();
    assert(trades[0].price == order1.price);
    assert(trades[0].quantity == 500);
    assert(trades[1].price == order2.price);
    assert(trades[1].quantity == 300);
    assert(orderBook.bestBid() ==  5023);
}

int main(){

    testOrderBook_RestingBidsIncomingSell_Filled();
    return 0;
}

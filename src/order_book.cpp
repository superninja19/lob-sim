#include <lob/order.hpp>
#include <lob/order_book.hpp>

namespace lob {

void OrderBook::addOrder(LimitOrder order){
    if (order.side == Side::Buy){
        auto& level = bidSideMap_[order.price];
        level.addOrder(order);
    }
    else if (order.side == Side::Sell){
        auto& level = askSideMap_[order.price];
        level.addOrder(order);
    }
}

std::optional<Price> OrderBook::bestBid() const{
    if (bidSideMap_.empty()){
        return std::nullopt;
    }
    return bidSideMap_.begin()->first;
}

std::optional<Price> OrderBook::bestAsk() const{
    if (askSideMap_.empty()){
        return std::nullopt;
    }
    return askSideMap_.begin()->first;
}

Quantity OrderBook::bidQuantityAt(Price price) const{
    if (auto search = bidSideMap_.find(price); search != bidSideMap_.end()){
        return search->second.getQuantity();
    }
    return 0;
}

Quantity OrderBook::askQuantityAt(Price price) const{
    if (auto search = askSideMap_.find(price); search != askSideMap_.end()){
        return search->second.getQuantity();
    }
    return 0;

}

// Does nothing but add new orders currently
OrderId OrderBook::submit(const OrderRequest& orderRequest){
    LimitOrder newOrder = {
        .orderId = allocateOrderId(),
        .price = orderRequest.price,
        .originalQuantity = orderRequest.quantity,
        .remainingQuantity = orderRequest.quantity,
        .side = orderRequest.side
    };

    addOrder(newOrder);
    return newOrder.orderId;
}

OrderId OrderBook::allocateOrderId(){
    auto ret = nextOrderId_;
    nextOrderId_++;
    return ret;
}

} // namespace lob

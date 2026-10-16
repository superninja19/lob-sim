#include <algorithm>

#include <lob/order.hpp>
#include <lob/order_book.hpp>

namespace lob {

Fill Level::fillFront(Quantity maxQuantity){
    assert(maxQuantity > 0 && "max quantity must be greater than 0");
    assert(!isEmpty() && "level can't be empty");

    auto& order = orders_.front();
    auto fill = std::min(order.remainingQuantity, maxQuantity);
    order.remainingQuantity -= fill;
    subtractQuantity(fill);

    Fill ret = {
        .restingOrderId = order.orderId,
        .price = order.price,
        .quantity = fill
    };

    if (order.remainingQuantity == 0){
        orders_.pop_front();
    }

    return ret;
}

void OrderBook::addOrder(LimitOrder order){
    switch (order.side)
    {
        case Side::Buy:
        {
            auto& bidLevel = bidSideMap_[order.price];
            bidLevel.addOrder(std::move(order));
            break;
        }

        case Side::Sell:
        {
            auto& askLevel = askSideMap_[order.price];
            askLevel.addOrder(std::move(order));
            break;
        }

        default:
        {
            break;
        }
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

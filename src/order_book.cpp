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
ExecutionResult OrderBook::submit(const OrderRequest& orderRequest){
    LimitOrder newOrder = {
        .orderId = allocateOrderId(),
        .price = orderRequest.price,
        .originalQuantity = orderRequest.quantity,
        .remainingQuantity = orderRequest.quantity,
        .side = orderRequest.side
    };

    ExecutionResult executionResult;
    executionResult.orderId = newOrder.orderId;
    executionResult.quantityRemaining = newOrder.remainingQuantity;

    if (newOrder.side == Side::Buy){
        do {
            auto frontLevelItr = askSideMap_.begin();
            if (frontLevelItr == askSideMap_.end()){
                addOrder(newOrder);
                return executionResult;
            };

            if (frontLevelItr->first > newOrder.price){
                addOrder(newOrder);

                if (executionResult.quantityFilled == 0){
                    executionResult.status = ExecutionStatus::Resting;
                }
                else{
                    executionResult.status = ExecutionStatus::PartiallyFilled;
                }
                return executionResult;
            }

            auto fill = frontLevelItr->second.fillFront(newOrder.remainingQuantity);
            newOrder.remainingQuantity -= fill.quantity;
            executionResult.quantityRemaining = newOrder.remainingQuantity;
            executionResult.quantityFilled += fill.quantity;

            Trade trade = {
                .tradeId = allocateTradeId(),
                .price = newOrder.price,
                .quantity = fill.quantity,
                .restingId = fill.restingOrderId,
                .aggressorId = newOrder.orderId,
                .aggressorSide = newOrder.side
            };

            executionResult.trades.push_back(trade);
            
            if (frontLevelItr->second.isEmpty()){
                frontLevelItr = askSideMap_.erase(frontLevelItr);
            }
        } while (newOrder.remainingQuantity > 0);
    }
    else{
        do {
            auto frontLevelItr = bidSideMap_.begin();
            if (frontLevelItr == bidSideMap_.end()){
                addOrder(newOrder);
                return executionResult;
            };

            if (frontLevelItr->first < newOrder.price){
                addOrder(newOrder);

                if (executionResult.quantityFilled == 0){
                    executionResult.status = ExecutionStatus::Resting;
                }
                else{
                    executionResult.status = ExecutionStatus::PartiallyFilled;
                }
                return executionResult;
            }

            auto fill = frontLevelItr->second.fillFront(newOrder.remainingQuantity);
            newOrder.remainingQuantity -= fill.quantity;
            executionResult.quantityRemaining = newOrder.remainingQuantity;
            executionResult.quantityFilled += fill.quantity;

            Trade trade = {
                .tradeId = allocateTradeId(),
                .price = newOrder.price,
                .quantity = fill.quantity,
                .restingId = fill.restingOrderId,
                .aggressorId = newOrder.orderId,
                .aggressorSide = newOrder.side
            };

            executionResult.trades.push_back(trade);
            
            if (frontLevelItr->second.isEmpty()){
                frontLevelItr = askSideMap_.erase(frontLevelItr);
            }
        } while (newOrder.remainingQuantity > 0);
    }

    executionResult.status = ExecutionStatus::Filled;
    executionResult.quantityFilled = newOrder.originalQuantity;
    return executionResult;
}

OrderId OrderBook::allocateOrderId(){
    auto ret = nextOrderId_;
    nextOrderId_++;
    return ret;
}

TradeId OrderBook::allocateTradeId(){
    auto ret = nextTradeId_;
    nextTradeId_++;
    return ret;
}

} // namespace lob

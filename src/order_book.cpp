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

void OrderBook::addTrade(Trade trade){
    assert(trade.quantity > 0 && "trade quantity must be greater than 0");
    assert(trade.price > 0 && "trade price must be greater than 0");
    trades_.push_back(trade);
}

const std::vector<Trade>& OrderBook::getTrades() const{
    return trades_;
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

    if (executionResult.quantityRemaining < 1 || newOrder.price <= 0){
        return executionResult;
    }

    if (newOrder.side == Side::Buy){
        do {
            auto frontLevelItr = askSideMap_.begin();
            if (frontLevelItr == askSideMap_.end() || frontLevelItr->first > newOrder.price){
                addOrder(newOrder);
                break;
            }

            auto fill = frontLevelItr->second.fillFront(newOrder.remainingQuantity);
            newOrder.remainingQuantity -= fill.quantity;
            executionResult.quantityRemaining = newOrder.remainingQuantity;
            executionResult.quantityFilled += fill.quantity;

            Trade trade = {
                .tradeId = allocateTradeId(),
                .price = frontLevelItr->first,
                .quantity = fill.quantity,
                .restingId = fill.restingOrderId,
                .aggressorId = newOrder.orderId,
                .aggressorSide = newOrder.side
            };

            executionResult.trades.push_back(trade);
            addTrade(trade);
            
            if (frontLevelItr->second.isEmpty()){
                askSideMap_.erase(frontLevelItr);
            }
        } while (newOrder.remainingQuantity > 0);
    }
    else{
        do {
            auto frontLevelItr = bidSideMap_.begin();
            if (frontLevelItr == bidSideMap_.end() || frontLevelItr->first < newOrder.price){
                addOrder(newOrder);
                break;
            }

            auto fill = frontLevelItr->second.fillFront(newOrder.remainingQuantity);
            newOrder.remainingQuantity -= fill.quantity;
            executionResult.quantityRemaining = newOrder.remainingQuantity;
            executionResult.quantityFilled += fill.quantity;

            Trade trade = {
                .tradeId = allocateTradeId(),
                .price = frontLevelItr->first,
                .quantity = fill.quantity,
                .restingId = fill.restingOrderId,
                .aggressorId = newOrder.orderId,
                .aggressorSide = newOrder.side
            };

            executionResult.trades.push_back(trade);
            addTrade(trade);
            
            if (frontLevelItr->second.isEmpty()){
                bidSideMap_.erase(frontLevelItr);
            }
        } while (newOrder.remainingQuantity > 0);
    }

    if (executionResult.quantityFilled == 0){
        executionResult.status = ExecutionStatus::Resting;
    }
    else if (executionResult.quantityRemaining > 0){
        executionResult.status = ExecutionStatus::PartiallyFilled;
    }
    else{
        executionResult.status = ExecutionStatus::Filled;
    }

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

#pragma once
#include <cassert>
#include <cstdint>
#include <functional>
#include <list>
#include <map>
#include <optional>
#include <utility>

#include <lob/order.hpp>
#include <lob/trade.hpp>

namespace lob {

struct Fill {
    OrderId restingOrderId = 0;
    Price price = 0;
    Quantity quantity = 0;
};

class Level{
public:
    Quantity getQuantity() const {
        return totalQuantity_;
    }

    bool isEmpty() const { return orders_.empty(); }

    void addOrder(LimitOrder order){
        assert(order.remainingQuantity > 0 && "remaining quantity must be greater than 0");
        auto quantity = order.remainingQuantity;
        orders_.push_back(std::move(order));
        addQuantity(quantity);
    }

    [[nodiscard]] Fill fillFront(Quantity maxQuantity);

private:
   void addQuantity(Quantity quantity){
        assert(quantity > 0 && "quantity must be greater than 0");
        totalQuantity_ += quantity;
    }

    void subtractQuantity(Quantity quantity){
        assert(quantity <= totalQuantity_ && "quantity must be less than or equal to total quantity");
        totalQuantity_ -= quantity;
    }

    std::list<LimitOrder> orders_;
    Quantity totalQuantity_ = 0;
};

class OrderBook{
public:
    void addTrade(Trade trade);
    std::optional<Price> bestBid() const;
    std::optional<Price> bestAsk() const;
    Quantity bidQuantityAt(Price price) const;
    Quantity askQuantityAt(Price price) const;
    ExecutionResult submit(const OrderRequest& orderRequest);

private:
    std::list<Trade> trades_;
    void addOrder(LimitOrder order);
    OrderId allocateOrderId();
    OrderId nextOrderId_ = 1;
    TradeId allocateTradeId();
    TradeId nextTradeId_ = 1;
    std::map<Price,Level> askSideMap_;
    std::map<Price,Level,std::greater<Price>> bidSideMap_;
};

} // namespace lob

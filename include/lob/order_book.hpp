#pragma once
#include <cassert>
#include <cstdint>
#include <functional>
#include <list>
#include <map>
#include <optional>

#include <lob/order.hpp>

namespace lob {

class Level{
public:
    Quantity getQuantity() const{
        return totalQuantity;
    }

    void addQuantity(Quantity quantity){
        assert(quantity > 0 && "quantity is not greater than 0");
        totalQuantity += quantity;
    }

    void subtractQuantity(Quantity quantity){
        assert(quantity <= totalQuantity && "quantity is not less than the total quantity");
        totalQuantity -= quantity;
    }

    void addOrder(LimitOrder order){
        assert(order.remainingQuantity > 0 && "remaining quantity is not greater than 0");
        auto quantity = order.remainingQuantity;
        orders.push_back(std::move(order));
        addQuantity(quantity);
    }

private:
    std::list<LimitOrder> orders;
    Quantity totalQuantity = 0;
};

class OrderBook{
public:
    std::optional<Price> bestBid() const;
    std::optional<Price> bestAsk() const;
    Quantity bidQuantityAt(Price price) const;
    Quantity askQuantityAt(Price price) const;
    OrderId submit(const OrderRequest& orderRequest);

private:
    void addOrder(LimitOrder order);
    OrderId allocateOrderId();
    OrderId nextOrderId_ = 1;
    std::map<Price,Level> askSideMap_;
    std::map<Price,Level,std::greater<Price>> bidSideMap_;
};

} // namespace lob

#pragma once
#include <cstdint>
#include <functional>
#include <list>
#include <map>
#include <optional>

#include <lob/order.hpp>

namespace lob {

struct Level{
    std::list<LimitOrder> orders;

    Quantity getQuantity() const{
        return totalQuantity;
    }

    void addQuantity(Quantity quantity){
        totalQuantity += quantity;
    }

    void subtractQuantity(Quantity quantity){
        // todo: should we throw here if quantity is greater than totalQuantity?
        totalQuantity -= quantity;
    }

private:
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
    void insertIntoLevel(LimitOrder order, Level& level);
    OrderId allocateOrderId();
    OrderId nextOrderId_ = 1;
    std::map<Price,Level> askSideMap_;
    std::map<Price,Level,std::greater<Price>> bidSideMap_;
};

} // namespace lob

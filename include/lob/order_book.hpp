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
    Quantity totalQuantity;
};

class OrderBook{
public:
    void addOrder(LimitOrder order);
    std::optional<Price> bestBid() const;
    std::optional<Price> bestAsk() const;
    Quantity bidQuantityAt(Price price) const;
    Quantity askQuantityAt(Price price) const;
    OrderId submit(const OrderRequest& orderRequest);

private:
    OrderId allocateOrderId();
    OrderId nextOrderId_ = 1;
    std::map<Price,Level> askSideMap_;
    std::map<Price,Level,std::greater<Price>> bidSideMap_;
};

} // namespace lob

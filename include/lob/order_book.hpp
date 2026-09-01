#pragma once
#include <list>
#include <map>
#include <optional>

#include <lob/order.hpp>

namespace lob {

using Level = std::list<LimitOrder>;
using Price = std::int64_t;
using OrderId = std::uint64_t;
using Quantity = std::int64_t;

class OrderBook{
public:
    void addOrder(LimitOrder order);
    std::optional<Price> bestBid();
    std::optional<Price> bestAsk();
    Quantity getQuantity(Price price);
    OrderId Submit(OrderRequest orderRequest);

private:
    OrderId nextOrderId = 1;
    std::map<Price,Level> askSideMap_;
    std::map<Price,Level,std::greater<Price>> bidSideMap_;
};

} // namespace lob

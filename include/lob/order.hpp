#pragma once
#include <cstdint>

namespace lob {

using Price = std::int64_t;
using OrderId = std::uint64_t;
using Quantity = std::int64_t;

enum class Side : std::uint8_t {
    Buy,
    Sell
};


struct LimitOrder {
    OrderId orderId;
    Price price;
    Quantity originalQuantity;
    Quantity remainingQuantity;
    Side side;
};

struct OrderRequest {
    Price price;
    Quantity quantity;
    Side side;
};

} // namespace lob

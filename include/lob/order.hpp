#pragma once
#include <cstdint>

namespace lob {

enum class Side : std::uint8_t {
    Ask,
    Bid
};


struct LimitOrder {
    std::uint64_t orderId;
    std::int64_t price;
    std::int64_t originalQuantity;
    std::int64_t remainingQuantity;
    Side side;
};

struct OrderRequest {
    std::int64_t price;
    std::int64_t quantity;
    Side side;
};

} // namespace lob

#pragma once
#include <cassert>
#include <cstdint>
#include <vector>

#include <lob/order.hpp>

namespace lob {

enum class ExecutionStatus : std::uint8_t {
    Rejected,
    Resting,
    PartiallyFilled,
    Filled
};

using TradeId = std::uint64_t;

struct Trade {
    TradeId tradeId;
    Price price;
    Quantity quantity;
    OrderId restingId;
    OrderId aggressorId;
    Side aggressorSide;
};

struct ExecutionResult {
    OrderId orderId_;
    Quantity quantityFilled_ = 0;
    Quantity quantityRemaining_ = 0;
    ExecutionStatus status_ = ExecutionStatus::Rejected;
    std::vector<Trade> trades_;
};

} // namespace lob

#pragma once
#include <cstdint>
#include <vector>

#include <lob/order.hpp>

namespace lob {

using TradeId = std::uint64_t;

enum class ExecutionStatus : std::uint8_t {
    Rejected,
    Resting,
    PartiallyFilled,
    Filled
};

struct Trade {
    TradeId tradeId = 0;
    Price price = 0;
    Quantity quantity = 0;
    OrderId restingId = 0;
    OrderId aggressorId = 0;
    Side aggressorSide;
};

struct ExecutionResult {
    OrderId orderId = 0;
    Quantity quantityFilled = 0;
    Quantity quantityRemaining = 0;
    ExecutionStatus status = ExecutionStatus::Rejected;
    std::vector<Trade> trades;
};

} // namespace lob

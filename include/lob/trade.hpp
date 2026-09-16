#pragma once
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
    Quantity quantityFilled;
    OrderId restingId;
    OrderId aggressorId;
    Side aggressorSide;
};

class ExecutionResult {

private:
    OrderId orderId;
    Quantity quantityFilled;
    Quantity quantityRemaining;
    ExecutionStatus status;
    std::vector<Trade> trades;
};

} // namespace lob

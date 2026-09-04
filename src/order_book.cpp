#include <lob/order.hpp>
#include <lob/order_book.hpp>

namespace lob {

    // todo: write helper for shared code below
    void OrderBook::addOrder(LimitOrder order){
        if (order.side == Side::Buy){
            bidSideMap_[order.price].orders.push_back(order); // Couldn't orders technically be pushed back out of order?
            bidSideMap_[order.price].totalQuantity += order.remainingQuantity;
        }
        else if (order.side == Side::Sell){
            askSideMap_[order.price].orders.push_back(order);
            askSideMap_[order.price].totalQuantity += order.remainingQuantity;
        }
    }

    std::optional<Price> OrderBook::bestBid() const{
        if (bidSideMap_.empty()){
            return std::nullopt;
        }
        return bidSideMap_.begin()->first;
    }

    std::optional<Price> OrderBook::bestAsk() const{
        if (askSideMap_.empty()){
            return std::nullopt;
        }
        return askSideMap_.begin()->first;
    }

    // todo: write helper function for shared quantity logic
    Quantity OrderBook::bidQuantityAt(Price price) const{
        if (auto search = bidSideMap_.find(price); search != bidSideMap_.end()){
            return search->second.totalQuantity;
        }
        return 0;
    }

    Quantity OrderBook::askQuantityAt(Price price) const{
        if (auto search = askSideMap_.find(price); search != askSideMap_.end()){
            return search->second.totalQuantity;
        }
        return 0;

    }

    // Does nothing but add new orders currently
    OrderId OrderBook::submit(const OrderRequest& orderRequest){
        LimitOrder newOrder = {
            .orderId = allocateOrderId(),
            .price = orderRequest.price,
            .originalQuantity = orderRequest.quantity,
            .remainingQuantity = orderRequest.quantity,
            .side = orderRequest.side
        };

        addOrder(newOrder);
        return newOrder.orderId;
    }

    OrderId OrderBook::allocateOrderId(){
        auto ret = nextOrderId_;
        nextOrderId_++;
        return ret;
    }

} // namespace lob

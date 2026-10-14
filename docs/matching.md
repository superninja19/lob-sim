# Matching Engine

How an incoming order is matched against the book.

1. Order request comes in (it says *I want to [BUY/SELL] X quantity for at worst Y price*). Create an order from it.
2. Check if the opposite side has a price level that crosses, starting from the front.
3. If a crossing price level exists, check the frontmost order in that level. Otherwise, place the aggressor order into the correct side as a resting order.
   1. Get the minimum remaining quantity between the two orders and subtract that number from both the resting and aggressor orders. Create a trade for the match that occurred and add it to the final execution result.
      1. If the resting order's remaining quantity is zero, pop the order from the level. If the level is now empty, delete the level. If the aggressor's remaining quantity is also zero, jump to step 4. If it is not zero, check whether the current level still exists: if it does, repeat step 3.1; if not, go to step 3.2.
      2. If the resting order's remaining quantity is not zero, jump to step 4.
   2. If the end of the level is reached, start from step 2 again.
4. If the order has no remaining quantity, the execution result is `Filled`. If the remaining quantity is less than the original but not zero, it is `PartiallyFilled`. If they are the same, it is `Resting`.

## Terms

- **Aggressor**: the incoming order that triggers matching.
- **Resting order**: an order already in the book, waiting to be matched.
- **Crosses**: the prices overlap, so a trade is possible. A buy crosses when `bestAsk <= buyLimit`; a sell crosses when `bestBid >= sellLimit`.
- **Trade price**: always the resting order's price.

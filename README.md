# lob-sim

A limit order book simulator in C++20.

## Build

    g++ -std=c++20 -Wall -Wextra -Wpedantic \
        -fsanitize=address,undefined -g \
        -Iinclude src/main.cpp && ./a.out

## Design decisions

1. No timestamp on Order. Priority is resolved by exchange-assigned sequence ID, which is monotonic by construction and avoids clock-resolution ties. Wall-clock time is not meaningful in a deterministic simulation; simulated time will be introduced with the event generator (v0.5).
2. No ticker name/symbol since the simulator is for a single instrument as is, no need to replicate the symbol on every copy of the order. It significantly increasing the memory size of the struct if using std::string as well, depending on memory layout of cache could increase number of cache misses.

## Roadmap

- [ ] v0.1 — Order representation
- [ ] v0.2 — Order book structure
- [ ] v0.3 — Matching engine
- [ ] v0.4 — Cancels & modifications
- [ ] v0.5 — Random order flow
- [ ] v0.6 — Strategy agents
- [ ] v0.7 — Logging & analytics
- [ ] v0.8 — Data integration

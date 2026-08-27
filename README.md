# lob-sim

A limit order book simulator in C++20.

## Build

    g++ -std=c++20 -Wall -Wextra -Wpedantic \
        -fsanitize=address,undefined -g \
        -Iinclude src/main.cpp && ./a.out

## Design decisions

_(recorded as they're made)_

## Roadmap

- [ ] v0.1 — Order representation
- [ ] v0.2 — Order book structure
- [ ] v0.3 — Matching engine
- [ ] v0.4 — Cancels & modifications
- [ ] v0.5 — Random order flow
- [ ] v0.6 — Strategy agents
- [ ] v0.7 — Logging & analytics
- [ ] v0.8 — Data integration

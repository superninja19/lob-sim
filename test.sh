#!/usr/bin/env bash
set -e
g++ -std=c++20 -Wall -Wextra -Wpedantic -fsanitize=address,undefined -g \
    -Iinclude src/order_book.cpp tests/*.cpp -o tests-lob-sim

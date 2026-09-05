#!/usr/bin/env bash
set -e
g++ -std=c++20 -Wall -Wextra -Wpedantic -fsanitize=address,undefined -g \
    -Iinclude src/*.cpp -o lob-sim

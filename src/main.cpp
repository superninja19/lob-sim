#include <iostream>

#include <lob/order.hpp>

int main(){

    lob::LimitOrder orderOne = {1, 50, 100, 100, lob::Side::Buy};
    lob::LimitOrder orderTwo = {2, 52, 200, 100, lob::Side::Sell};
    lob::LimitOrder orderThree = {3, 51, 10, 5, lob::Side::Buy};

    std::cout << "We have orders: " << orderOne.orderId << ", " << orderTwo.orderId << ", " << orderThree.orderId;

    return 0;
}


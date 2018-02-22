//
// Created by v on 22.02.18.
//

#ifndef RECIVER_USER_H
#define RECIVER_USER_H

#include <string>
#include <chrono>


struct User {
    using Id = long long;
    Id id = 0;
    std::string name;
    double wonAmount = 0;
};


struct Bid {
    double amount;
    std::chrono::time_point<std::chrono::steady_clock> time;
};


#endif //RECIVER_USER_H

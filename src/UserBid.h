//
// Created by v on 22.02.18.
//

#ifndef RECIVER_USER_H
#define RECIVER_USER_H

#include <string>
#include <chrono>
#include <sstream>


struct User {
    using Id = long long;
    User(Id id, std::string name, double wonAmount = 0) :
    id(id), name(name), wonAmount(wonAmount) { }
    Id id = 0;
    std::string name;
    double wonAmount = 0;
    static std::string to_string(const User &user);
};




struct Bid {
    double amount;
    std::chrono::time_point<std::chrono::steady_clock> time;
};


#endif //RECIVER_USER_H

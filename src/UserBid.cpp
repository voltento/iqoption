//
// Created by v on 22.02.18.
//

#include "UserBid.h"

std::string User::to_string(const User &user) {
    return std::string("UserId: " + std::to_string(user.id) +
                       " Name: " + user.name + " WonAmount: " + std::to_string(user.wonAmount));
}

//
// Created by v on 2/21/18.
//

#ifndef RECIVER_SESSION_H
#define RECIVER_SESSION_H

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

#include "UserDataMgr.h"


class Session
{
public:
    explicit Session(boost::asio::ip::tcp::socket socket, UserDataMgr* session, std::chrono::milliseconds sendPeriod);
    void start();
    ~Session();
private:
    void doWrite();
private:
    boost::asio::ip::tcp::socket socket;
    std::string data;
    UserDataMgr* storage;
    std::chrono::milliseconds sendPeriod;
};


#endif //RECIVER_SESSION_H

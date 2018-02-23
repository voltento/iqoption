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


class Session {
public:
    explicit Session(boost::asio::ip::tcp::socket socket, UserDataMgr *session,
                     size_t sendTimeOut, boost::asio::io_service& io_service);

    void start();

    ~Session();

    bool IsAlive() const { return isAlive.load(); }

private:
    void doWrite();

private:
    const boost::posix_time::seconds sendPeriod;
    boost::asio::ip::tcp::socket socket;
    std::string data;
    UserDataMgr *storage;
    boost::asio::io_service& io_service;
    boost::asio::deadline_timer timer;

    std::atomic_bool isAlive;
};


#endif //RECIVER_SESSION_H

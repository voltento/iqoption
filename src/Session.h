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

    void Start();

    ~Session();

    bool IsAlive() const { return isAlive.load(); }

private:
    void Stop();
    void StartWriteStat(const boost::system::error_code& error, std::size_t bytesTransffered);
    void DoWrite();
    bool BuildStat(std::string& out);
    void WriteHandler(const boost::system::error_code& ec, std::size_t bytes_transferred);
private:
    const boost::posix_time::seconds sendPeriod;
    boost::asio::ip::tcp::socket socket;
    UserDataMgr *dataMgr = nullptr;
    boost::asio::io_service& io_service;
    boost::asio::deadline_timer timer;

    std::atomic_bool isAlive;
    static constexpr size_t idSize = 8;
    User::Id userId = 0;
    char readBuffer[idSize];
    std::string writeBuffer;
};


#endif //RECIVER_SESSION_H

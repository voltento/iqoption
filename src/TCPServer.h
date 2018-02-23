//
// Created by v on 2/21/18.
//

#ifndef RECIVER_TCPSERVER_H
#define RECIVER_TCPSERVER_H

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <chrono>

#include "Session.h"
#include "UserDataMgr.h"

using boost::asio::ip::tcp;


class TCPServer {
public:
    /// Start tcp server
    /// \idlexcept runtime_error
    TCPServer(boost::asio::io_service &io_service, unsigned short port, UserDataMgr *storage,
              size_t sendPeriod);

    ~TCPServer();

    void Stop();

private:
    void doAccept();
    void clearSessions();

    std::mutex sessionsMutex;
    std::list<std::unique_ptr<Session>> sessions;

    boost::asio::deadline_timer clearSessionTimer;
    boost::posix_time::seconds clearSessionPeriod = boost::posix_time::seconds(3);

    boost::asio::io_service &io_service;
    tcp::acceptor acceptor;
    tcp::socket socket;
    UserDataMgr *storage;
    size_t sendPeriodSeconds = 60;
};


#endif //RECIVER_TCPSERVER_H

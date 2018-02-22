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
              std::chrono::milliseconds sendPeriod);

    ~TCPServer();

    void Stop();

private:
    void doAccept();

    tcp::acceptor acceptor;
    tcp::socket socket;
    UserDataMgr *storage;
    std::chrono::milliseconds sendPeriod = std::chrono::milliseconds(60);
};


#endif //RECIVER_TCPSERVER_H

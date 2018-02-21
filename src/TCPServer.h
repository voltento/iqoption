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

#include "Session.h"

using boost::asio::ip::tcp;

class TCPServer
{
public:
    TCPServer(boost::asio::io_service& io_service, unsigned short port);

private:
    void doAccept();

    tcp::acceptor acceptor;
    tcp::socket socket;
};


#endif //RECIVER_TCPSERVER_H

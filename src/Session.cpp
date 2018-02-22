//
// Created by v on 2/21/18.
//

#include "Session.h"

void Session::doWrite() {
    socket.write_some(boost::asio::buffer(data.c_str(), data.size()));
}

void Session::start() {
    doWrite();
}

Session::Session(tcp::socket socket)
        : socket(std::move(socket))
{
}

Session::~Session() {
    socket.close();
    std::cout << "Session closed" << std::endl;
}

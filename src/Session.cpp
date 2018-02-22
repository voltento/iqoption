//
// Created by v on 2/21/18.
//

#include "Session.h"

using namespace boost::asio::ip;


Session::Session(tcp::socket socket, UserDataMgr *storage, std::chrono::milliseconds sendPeriod):
        socket(std::move(socket)),
        storage(storage),
        sendPeriod(sendPeriod)
{ }

void Session::doWrite() {
    socket.write_some(boost::asio::buffer(data.c_str(), data.size()));
}

void Session::start() {
    doWrite();
    std::this_thread::sleep_for(sendPeriod);
}

Session::~Session() {
    socket.close();
    std::cout << "Session closed" << std::endl;
}

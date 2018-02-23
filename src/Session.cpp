//
// Created by v on 2/21/18.
//

#include "Session.h"
#include <boost/bind.hpp>


using namespace boost::asio::ip;


Session::Session(tcp::socket socket, UserDataMgr *storage, size_t sendSeconds,
                 boost::asio::io_service &io_service) :
        socket(std::move(socket)),
        storage(storage),
        sendPeriod(boost::posix_time::seconds(sendSeconds)),
        io_service(io_service),
        timer(io_service, sendPeriod),
        isAlive(true)
{}

void Session::doWrite() {
    std::this_thread::get_id();
    std::stringstream ss;
    ss << std::this_thread::get_id();
    ss >> data;
    try {
        socket.write_some(boost::asio::buffer(data.c_str(), data.size()));
    }
    catch (const boost::system::system_error& ex) {
        std::cerr << "Write socket exception occured: '" << ex.what() << std::endl;
        isAlive.store(false);
        return;
    }
    timer.expires_at(timer.expires_at() + sendPeriod);
    timer.async_wait(boost::bind(&Session::doWrite, this));
}

void Session::start() {
    doWrite();
}

Session::~Session() {
    timer.cancel();
    socket.close();
    std::cout << "Session closed" << std::endl;
}

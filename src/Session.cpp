//
// Created by v on 2/21/18.
//

#include "Session.h"
#include <boost/bind.hpp>


using namespace boost::asio::ip;


Session::Session(tcp::socket socket, UserDataMgr *storage, size_t sendSeconds,
                 boost::asio::io_service &io_service) :
        socket(std::move(socket)),
        dataMgr(storage),
        sendPeriod(boost::posix_time::seconds(sendSeconds)),
        io_service(io_service),
        timer(io_service),
        isAlive(true)
{}

void Session::DoWrite() {
    if(!BuildStat(writeBuffer)) {
        Stop();
        return;
    }

    try {
        socket.async_write_some(boost::asio::buffer(writeBuffer.c_str(), writeBuffer.size()),
                                boost::bind(&Session::WriteHandler, this, _1, _2));
    }
    catch (const boost::system::system_error& ex) {
        std::cerr << "Write socket exception occured: '" << ex.what() << std::endl;
        Stop();
        return;
    }
    timer.expires_from_now(sendPeriod);
    timer.async_wait(boost::bind(&Session::DoWrite, this));
}

void Session::Start() {
    boost::asio::async_read(socket,
                            boost::asio::buffer(readBuffer, sizeof(readBuffer)),
                            boost::bind(&Session::StartWriteStat, this, _1, _2));
}

Session::~Session() {
    Stop();
}

void Session::StartWriteStat(const boost::system::error_code& error, std::size_t bytesTransffered) {
    if (!error && bytesTransffered == idSize)
    {
        try {
            userId = *reinterpret_cast<User::Id*>(readBuffer);
        }
        catch (const std::exception& ex) {
            std::cerr << "Parse id exception. Raw id: '" <<  std::string(readBuffer, idSize) << "'" << std::endl;
        }
        if(dataMgr->DoesUserExist(userId)) {
            DoWrite();
        }
        else {
            std::cerr << "Unrigestred user try to connect. User id: '" << userId << "'" << std::endl;
            Stop();
        }
    }
    else {
        std::cerr << "Read from socket end with error: '" << error << "'" << std::endl;
        Stop();
    }
}

void Session::Stop() {
    if(isAlive.load())
        return;
    timer.cancel();
    socket.close();
    std::cout << "Session closed. User id: '" << userId << "'" << std::endl;
    isAlive.store(false);
}

bool Session::BuildStat(std::string & data) {
    return dataMgr->BuildStat(userId, data);
}

void Session::WriteHandler(const boost::system::error_code &ec, std::size_t bytes_transferred) {
    std::cerr << "Send error occured: '" << ec.message() << "'" << std::endl;
    Stop();
}

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

using boost::asio::ip::tcp;

class Session
{
public:
    explicit Session(tcp::socket socket);
    void start();
    ~Session();
private:
    void doWrite();
private:
    tcp::socket socket;
    std::string data;
};


#endif //RECIVER_SESSION_H

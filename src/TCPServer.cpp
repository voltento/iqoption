//
// Created by v on 2/21/18.
//

#include "TCPServer.h"

#include <boost/bind.hpp>

TCPServer::TCPServer(boost::asio::io_service &io_service, unsigned short port, UserDataMgr *storage,
                     size_t sendPeriod)
        : acceptor(io_service, tcp::endpoint(tcp::v4(), port)),
          socket(io_service),
          storage(storage), sendPeriodSeconds(sendPeriod), io_service(io_service),
          clearSessionTimer(io_service) {
    if (storage == nullptr)
        throw std::runtime_error("Nullptr was sent as storage'");

    doAccept();
    clearSessions();
    }

void TCPServer::doAccept() {
    acceptor.async_accept(socket,
                          [this](boost::system::error_code ec) {
                              if (!ec) {
                                  std::unique_lock<std::mutex> guard(sessionsMutex);
                                  sessions.emplace_back(
                                          std::make_unique<Session>(std::move(socket), storage,
                                                                    sendPeriodSeconds, io_service)
                                  );
                                  Session* newSession = sessions.rbegin()->get();
                                  guard.unlock();
                                  newSession->start();
                              }
                              doAccept();
                          });
}

void TCPServer::Stop() {
    clearSessionTimer.cancel();
    socket.close();
}

TCPServer::~TCPServer() {
    Stop();
}

void TCPServer::clearSessions() {
    std::lock_guard<std::mutex> guard(sessionsMutex);
    auto it = std::remove_if(std::begin(sessions), std::end(sessions),
                             [](const std::unique_ptr<Session> &session) {
                                 return !session->IsAlive();
                             });
    sessions.erase(it, sessions.end());
    std::cerr << "Clear sessions..." << std::endl;
    clearSessionTimer.async_wait(boost::bind(&TCPServer::clearSessionTimer, this));
    clearSessionTimer.expires_at(clearSessionTimer.expires_at() + clearSessionPeriod);
}

//
// Created by v on 2/21/18.
//

#include "TCPServer.h"

TCPServer::TCPServer(boost::asio::io_service &io_service, unsigned short port)
        : acceptor(io_service, tcp::endpoint(tcp::v4(), port)),
          socket(io_service)
{
    doAccept();
}

void TCPServer::doAccept() {
    acceptor.async_accept(socket,
                          [this](boost::system::error_code ec)
                          {
                              if (!ec)
                              {
                                  std::make_shared<Session>(std::move(socket))->start();
                              }

                              doAccept();
                          });
}

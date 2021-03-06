#include <iostream>
#include <list>
#include <chrono>

#include "cxxopts.hpp"

#include "RawCmdDataProvider.h"
#include "UserDataMgr.h"
#include "TCPServer.h"


std::unique_ptr<TCPServer> server;
std::unique_ptr<boost::asio::io_service> io_service;
std::unique_ptr<UserDataMgr> dataMgr;

void HandleIntSig(int signal) {
    std::cout << "Handle SigInt" << std::endl;
    server.reset();
    if (dataMgr)
        dataMgr->Stop();
    io_service->stop();
}

int main(int argc, char **argv) {
    std::signal(SIGINT, HandleIntSig);

    cxxopts::Options argOptions("Receiver", "Add description");
    argOptions.add_options()
            ("source", "Command source file in like csv format.", cxxopts::value<std::string>())
            ("p,port", "Service port number", cxxopts::value<int>())
            ("period", "Send statistic period in seconds", cxxopts::value<size_t>());

    std::unique_ptr<cxxopts::ParseResult> result;

    try {
        result = std::make_unique<cxxopts::ParseResult>(std::move(argOptions.parse(argc, argv)));
    }
    catch (const std::exception &ex) {
        std::cerr << ex.what() << std::endl;
        std::cerr << argOptions.help() << std::endl;
        return 1;
    }
    cxxopts::ParseResult resultRef = *result;

    std::string source;
    if (resultRef.count("source") == 0) {
        std::cerr << "Required parameter isn't provided: 'source'" << std::endl;
        std::cerr << argOptions.help() << std::endl;
        return 1;
    } else {
        source = resultRef["source"].as<std::string>();
    }

    int port = -1;
    if (resultRef.count("port") == 0) {
        std::cerr << "Required parameter isn't provided: 'port'" << std::endl;
        std::cerr << argOptions.help() << std::endl;
        return 1;
    } else {
        port = resultRef["port"].as<int>();
    }

    size_t sendPeriod = 60;
    if (resultRef.count("period") > 0) {
        sendPeriod = resultRef["period"].as<size_t>();
    }

    io_service = std::make_unique<boost::asio::io_service>();
    DataProvider dataProvider(source.c_str());
    dataMgr = std::make_unique<UserDataMgr>(&dataProvider, *io_service);
    std::thread dataMgrThread([&]() { dataMgr->Start(); });

    server = std::make_unique<TCPServer>(*io_service,
                                         static_cast<unsigned short>(port),
                                         dataMgr.get(),
                                         sendPeriod);

    std::cout << "Start listening on port: '" << port << "'" << std::endl;
    std::vector<std::thread> threadPool;
    for(int i = 0; i < 10; ++i)
        threadPool.emplace_back([&](){io_service->run();});
    for(auto& t : threadPool)
        t.join();
    if(dataMgrThread.joinable())
        dataMgrThread.join();
    std::cout << "Finished." << std::endl;
    return 0;
}
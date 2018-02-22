#include <iostream>
#include <list>
#include <chrono>

#include "cxxopts.hpp"



int main(int argc, char** argv) {

    cxxopts::Options argOptions("Receiver", "Add description");
    argOptions.add_options()
            ("source", "Command source file in like csv format.", cxxopts::value<std::string>())
            ("p,port", "Service port number", cxxopts::value<int>())
            ("period", "Send statistic period in seconds", cxxopts::value<size_t>())
            ;

    std::unique_ptr<cxxopts::ParseResult> result;

    try {
        result = std::make_unique<cxxopts::ParseResult>(std::move(argOptions.parse(argc, argv)));
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        std::cerr << argOptions.help() << std::endl;
        return 1;
    }
    cxxopts::ParseResult resultRef = *result;

    std::string source;
    if(resultRef.count("source") == 0) {
        std::cerr << "Required parameter isn't provided: 'source'" << std::endl;
        std::cerr << argOptions.help() << std::endl;
        return 1;
    } else {
        source = resultRef["source"].as<std::string>();
    }

    int port = -1;
    if(resultRef.count("port") == 0) {
        std::cerr << "Required parameter isn't provided: 'port'" << std::endl;
        std::cerr << argOptions.help() << std::endl;
        return 1;
    } else {
        port = resultRef["port"].as<int>();
    }

    std::chrono::seconds period(60);
    if(resultRef.count("period") > 0) {
        period = std::chrono::seconds(resultRef["period"].as<int>());
    }




    return 0;
}
#include <iostream>
#include "cxxopts.hpp"

cxxopts::Options InitCppOpts() {
    cxxopts::Options options("Receiver", "Add description");
    options.add_options()
            ("s,source", "Command source file in like csv format.", cxxopts::value<std::string>())
            ("stdin", "Use stdin as source")
            ("stdout", "Use stdout as consumer")
            ("out_file", "Use the out_file as consumer", cxxopts::value<std::string>())
            ("our_tcp", "Use the out_tcp as consumer. Argument format: [host]:[port]", cxxopts::value<std::string>())
            ;
    return options;
}

int main(int argc, char** argv) {
    cxxopts::Options argOptions = InitCppOpts();
    auto result = argOptions.parse(argc, argv);
    if(result.count("s") == 0) {
        std::cerr << argOptions.help() << std::endl;
        return 1;
    }
    return 0;
}
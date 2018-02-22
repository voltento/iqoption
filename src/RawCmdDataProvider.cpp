//
// Created by v on 21.02.18.
//

#include <iostream>
#include <cstring>
#include "RawCmdDataProvider.h"

DataProvider::DataProvider(const char *sourceFile) {
    if(sourceFile == nullptr)
        throw std::runtime_error("Nullptr was sent as source file'");

    source.open(sourceFile, std::ifstream::in);
    if (!source.is_open()) {
        throw std::runtime_error(std::string("Can't open source file: '") + sourceFile + "'");
    }
}

DataProvider::~DataProvider() {
    source.close();
    if(readThread && readThread->joinable())
        readThread->join();
}

void DataProvider::StartAsynchRead() {
    readThread = std::make_unique<std::thread>(
        [&](){
            std::string newData;
            while(Read(newData)) {
                if(MAX_QUEUE_SIZE >= rawQueries.size()) {
                    std::unique_lock<std::mutex> guard(queueMutex);
                    halfQueueFree.wait(guard, [&](){ return rawQueries.size() <= MAX_QUEUE_SIZE/2; });
                }
                std::unique_lock<std::mutex> lockGuard(queueMutex);
                rawQueries.push(std::move(newData));
                if(rawQueries.size() == 1)
                    newDataAdded.notify_one();
            }
        }
    );
}

bool DataProvider::Next(std::string &out) {
    std::unique_lock<std::mutex> lockGuard(queueMutex);

    if(rawQueries.empty())
        return false;

    out = rawQueries.front();
    rawQueries.pop();

    if(rawQueries.size() <= MAX_QUEUE_SIZE/2)
        halfQueueFree.notify_one();

    return true;
}

bool DataProvider::Read(std::string &out) {
    if (source.good()) {
        std::getline(source, out);
        return true;
    }
    return false;
}

bool DataProvider::WaitNoEmpty() const {
    std::unique_lock<std::mutex> guard(queueMutex);
    newDataAdded.wait(guard, [&](){ return !rawQueries.empty();});
    return false;
}

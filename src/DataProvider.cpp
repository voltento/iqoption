//
// Created by v on 21.02.18.
//

#include "DataProvider.h"

bool DataProvider::Next(std::string &out) {
    return rawQueries.pop(out);
}

void DataProvider::WaitNewData() const {
    mtx.lock();
}

DataProvider::DataProvider(const char* sourceFile) {
    source = std::fopen(sourceFile, "r");
    if (source == nullptr) {
        throw std::runtime_error(std::string("Can't open source file: ;") + sourceFile + "'");
    }
}

void DataProvider::SetMsgBufferCount(std::uint32_t newSize) {
    if (newSize == 0)
        newSize = 1;
    msgBuferCount = newSize;
}

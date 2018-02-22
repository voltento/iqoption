//
// Created by v on 21.02.18.
//

#ifndef RECIVER_DATAPROVIDER_H
#define RECIVER_DATAPROVIDER_H

#include <boost/lockfree/queue.hpp>
#include <mutex>
#include <chrono>
#include <cstdio>

/// Message data provider. Might work in separate thread.
class DataProvider {
public:
    /// \idlexcept std::runtime_error
    explicit DataProvider(const char* sourceFile);

    bool Next(std::string& out);

    void WaitNewData() const;

    template <class _Rep, class _Period>
    bool WaitNewDataFor(const std::chrono::duration<_Rep, _Period>& time) { return mtx.try_lock_for(time); }

    void SetMsgBufferCount(std::uint32_t newSize);

private:
    boost::lockfree::queue<std::string> rawQueries;
    mutable std::timed_mutex mtx;

    std::FILE* source;
    std::uint32_t msgBuferCount = 10000;
};


#endif //RECIVER_DATAPROVIDER_H

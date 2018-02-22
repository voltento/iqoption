//
// Created by v on 21.02.18.
//

#ifndef RECIVER_DATAPROVIDER_H
#define RECIVER_DATAPROVIDER_H

#include <mutex>
#include <chrono>
#include <cstdio>
#include <string>
#include <queue>
#include <atomic>
#include <fstream>
#include <condition_variable>
#include <thread>

/// Message data provider. Might work in separate thread.
class DataProvider {
public:
    /// \idlexcept std::runtime_error
    explicit DataProvider(const char* sourceFile);
    ~DataProvider();

    bool Next(std::string& out);

    /// Waits while data will be added to empty queue.
    /// \return False if new data will not be provided more, other false
    bool WaitNoEmpty(const std::chrono::milliseconds& timeout) const;

    void StartAsynchRead();
private:
    bool Read(std::string& out);
private:
    std::ifstream source;
    std::queue<std::string> rawQueries;

    mutable std::mutex queueMutex;
    mutable std::condition_variable newDataAdded;
    mutable std::condition_variable halfQueueFree;

    std::unique_ptr<std::thread> readThread;

    static constexpr size_t MAX_QUEUE_SIZE = 1000;
};


#endif //RECIVER_DATAPROVIDER_H

//
// Created by v on 21.02.18.
//

#ifndef RECIVER_STATSERVICE_H
#define RECIVER_STATSERVICE_H


#include <chrono>


class StatService {
public:
    StatService(int port, std::chrono::seconds sendPeriod);
private:

};


#endif //RECIVER_STATSERVICE_H

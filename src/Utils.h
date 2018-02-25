//
// Created by v on 2/24/18.
//

#ifndef RECIVER_UTILS_H
#define RECIVER_UTILS_H

#include <ctime>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace TimeUtils {
    using namespace boost::gregorian;

    enum Week {PAST, CUR_WEEK, NEXT_WEEK, FUTURE};
    Week DetectWeek(time_t linuxTime);
}

#endif //RECIVER_UTILS_H

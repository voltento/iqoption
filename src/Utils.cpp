//
// Created by v on 2/25/18.
//

#include "Utils.h"

namespace TimeUtils {

Week DetectWeek(time_t linuxTime) {
    boost::gregorian::date today(day_clock::local_day());
    date prevMonday = previous_weekday(today, greg_weekday(boost::gregorian::Monday));
    date nextMonday = next_weekday(today, greg_weekday(boost::gregorian::Monday));
    date nextNextMonday = next_weekday(nextMonday, greg_weekday(boost::gregorian::Monday));
    date time = boost::posix_time::from_time_t(linuxTime).date();
    if(time > prevMonday) {
        if(time > nextMonday) {
            return (time < nextNextMonday ? NEXT_WEEK : FUTURE);
        } else
            return CUR_WEEK;
    }
    else
        return PAST;
}

}
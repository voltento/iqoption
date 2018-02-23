//
// Created by v on 22.02.18.
//

#ifndef RECIVER_STORAGE_H
#define RECIVER_STORAGE_H

#include <unordered_map>
#include <unordered_set>
#include <list>
#include <string>
#include <boost/algorithm/string/split.hpp>
#include <utility>
#include <tuple>
#include <set>

#include "RawCmdDataProvider.h"
#include "UserBid.h"

class UserDataMgr {
    struct UserSorter {
        bool operator()(const User *u1, const User *u2) {
            static constexpr double delta = 0.000001;
            if (std::abs(u1->wonAmount - u2->wonAmount) < delta)
                return u1->id > u2->id;

            return u1->wonAmount > u2->wonAmount;
        }
    };

    enum class Command : int {
        INVALID = 0, REGISTRED = 1, DEAL_WON = 4
    };
public:
    explicit UserDataMgr(DataProvider *dataProvider);

    void Start();

    void Stop();

    bool DoesUserExist(User::Id userId) const;

    bool BuildStat(User::Id userId, std::string &data);

private:
    /// Use this method only under mutex
    std::vector<std::string> BuildNStats(std::set<const User *, UserSorter>::iterator it, size_t itInd, const size_t num);

    std::vector<std::string> BuildNAroundUser(User* id, const size_t num);

    void FillFromDataProvider();

    void RegisterateUser(User::Id userId, std::string &&name);

    void UserDealWon(User::Id userId, const std::string &time, const std::string &amount);

    void UpdateUserSorted(const User *user);

    /// Parses raw command.
    std::tuple<Command, User::Id, std::list<std::string>> ParseCommand(const std::string &rawCommand);

private:
    DataProvider *dataProvider;

    std::unordered_map<User::Id, std::unique_ptr<User>> users;

    std::set<const User *, UserSorter> userSortedAmount;

    mutable std::mutex usersMutex;

    std::atomic_bool isStopped;


    static constexpr char SPLITER = '\t';
    static constexpr size_t NUM_STAT_POSITION_PRINT = 10;
};


#endif //RECIVER_STORAGE_H

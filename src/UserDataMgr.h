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

#include "RawCmdDataProvider.h"
#include "UserBid.h"

class UserDataMgr {
     enum class Command  : int {INVALID= 0, REGISTRED = 1, CONNECTED = 2,  DISCONNECTED = 3, DEAL_WON = 4};
public:
    explicit UserDataMgr(DataProvider* dataProvider);
    void Start();
    void Stop();
private:
    void FillFromDataProvider();

    void ProcCommand(const std::pair<Command, std::list<std::string>>& cmdArgs);

    // Commands processing methods

    void ConnectUser(User::Id userId);
    void DisconnectUser(User::Id userId);
    void RegistrateUser(User::Id userId, std::string&& name);
    void UserDealWon(User::Id userId, const std::string& time, const std::string& amount);

    /// Parses raw command.
    std::tuple<Command, User::Id, std::list<std::string>> ParseCommand(const std::string& rawCommand);
private:
    DataProvider* dataProvider;

    std::unordered_map<User::Id, User> users;
    std::unordered_set<User::Id> connectedUsers;

    std::atomic_bool isStopped;

    mutable std::mutex usersMutex;

    static constexpr char SPLITER = '\t';
};


#endif //RECIVER_STORAGE_H

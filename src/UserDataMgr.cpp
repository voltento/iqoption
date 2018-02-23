//
// Created by v on 22.02.18.
//

#include <iostream>
#include "UserDataMgr.h"

UserDataMgr::UserDataMgr(DataProvider *dataProvider) : dataProvider(dataProvider) {
    if (dataProvider == nullptr)
        throw std::runtime_error("Nullptr was sent as dataProvider'");
}

void UserDataMgr::Start() {
    isStopped.store(false);
    dataProvider->StartAsynchRead();
    FillFromDataProvider();
}


void UserDataMgr::FillFromDataProvider() {
    std::string rawCommand;
    while (true) {
        if (isStopped.load())
            return;

        if (!dataProvider->Next(rawCommand)) {
            static constexpr std::chrono::milliseconds timeout(10);
            while (!dataProvider->WaitNoEmpty(timeout)) {
                if (isStopped.load())
                    return;
            }
        } else {
            std::tuple<Command, User::Id, std::list<std::string>> cmdArg = ParseCommand(rawCommand);
            switch (std::get<0>(cmdArg)) {
                break;
                case Command::DEAL_WON: {
                    const std::string &timeRaw = *std::get<2>(cmdArg).begin();
                    const std::string &amount = *std::next(std::get<2>(cmdArg).begin());
                    UserDealWon(std::get<1>(cmdArg), timeRaw, amount);
                }
                    break;
                case Command::REGISTRED: {
                    RegisterateUser(std::get<1>(cmdArg), std::move(*std::get<2>(cmdArg).begin()));
                }
                    break;
                case Command::INVALID:
                    break;
            }
        }
    }
}

std::tuple<UserDataMgr::Command, User::Id, std::list<std::string>>
UserDataMgr::ParseCommand(const std::string &rawCommand) {
    std::list<std::string> args;
    boost::algorithm::split(args, rawCommand, [](char v) { return SPLITER == v; });
    Command command = Command::INVALID;
    if (args.size() < 2) {
        std::cerr << "Can't parse command: '" << rawCommand << "'" << std::endl;
        return std::make_tuple(command, 0, args);
    }

    auto LogCantParseCommand = [&](size_t expectedArgumentCount, size_t providedArgumentCount) {
        std::cerr << "Can't parse command: '" << rawCommand
                  << "' Wrong argument count. "
                  << "Expected: '" << expectedArgumentCount << "' Provided: '" << providedArgumentCount << std::endl;
    };

    std::string commandNameRaw = std::move(*args.begin());
    args.pop_front();

    User::Id userId = 0;
    try {
        userId = std::stoll(args.front());
    }
    catch (const std::invalid_argument &ex) {
        std::cerr << "Can't parse use id from : '" << args.front() << "'. Reason: '" << ex.what() << "'" << std::endl;
        return std::make_tuple(command, 0, args);
    }
    args.pop_front();

    if (commandNameRaw == "user_registered") {
        if (args.size() != 1)
            LogCantParseCommand(1, args.size());
        else
            command = Command::REGISTRED;
    } else if (commandNameRaw == "user_deal_won") {
        if (args.size() != 2)
            LogCantParseCommand(2, args.size());
        else
            command = Command::DEAL_WON;
    }

    return std::make_tuple(command, userId, args);
}

void UserDataMgr::RegisterateUser(User::Id userId, std::string &&name) {
    std::lock_guard<std::mutex> guard(usersMutex);
    auto it = users.find(userId);
    if (it != users.end()) {
        std::cerr << "Try register for already registered user. UserId: '" << userId << "'" << std::endl;
        return;
    }
    bool pointersInvalid = users.load_factor() * users.bucket_count() == users.size();
    auto newUserSPtr = std::make_unique<User>(userId, std::move(name));
    auto* newUserPtr = newUserSPtr.get();
    users[userId] = std::move(newUserSPtr);
    if (!users.empty() && pointersInvalid) {
        userSortedAmount.clear();
        for(const auto& user : users) {
            userSortedAmount.insert(user.second.get());
        }
    } else {
        UpdateUserSorted(newUserPtr);
    }
}

void UserDataMgr::UserDealWon(User::Id userId, const std::string &time, const std::string &amountRaw) {
    std::lock_guard<std::mutex> guard(usersMutex);
    auto it = users.find(userId);
    if (it == users.end()) {
        std::cerr << "Can't find user. UserId: '" << userId << "'" << std::endl;
        return;
    }

    double amount = 0;
    try {
        amount = std::stod(amountRaw);
    }
    catch (const std::invalid_argument &ex) {
        std::cerr << "Can't parse use id from : '" << amountRaw << "'. Reason: '" << ex.what() << "'" << std::endl;
        return;
    }

    it->second->wonAmount += amount;
    UpdateUserSorted(it->second.get());
}

void UserDataMgr::Stop() {
    isStopped.store(true);
}

void UserDataMgr::UpdateUserSorted(const User *user) {
    auto userSortedIt = userSortedAmount.find(user);
    if (userSortedIt != userSortedAmount.end()) {
        auto removeIterator = userSortedIt;
        userSortedIt = std::next(removeIterator);
        userSortedAmount.erase(removeIterator);
    }

    userSortedAmount.insert(user);
}

bool UserDataMgr::DoesUserExist(User::Id userId) const {
    std::lock_guard<std::mutex> guard(usersMutex);
    return users.find(userId) != users.end();
}

bool UserDataMgr::BuildStat(User::Id userId, std::string &data) {
    std::lock_guard<std::mutex> guard(usersMutex);
    auto it = users.find(userId);
    if (it == users.end())
        return false;

    for (auto &stat : BuildNStats(userSortedAmount.begin(), 0, NUM_STAT_POSITION_PRINT))
        data.append(stat);

    return true;
}

std::vector<std::string> UserDataMgr::BuildNStats(std::set<const User*, UserSorter>::iterator it, size_t itInd, const size_t num) {
    std::vector<std::string> results;
    results.reserve(num);
    for (size_t counter = 0;
         it != userSortedAmount.end() && counter < num;
         ++it, ++counter
            ) {
        std::string newResult = "Position: " + std::to_string(counter+itInd) + " ";
        newResult += User::to_string(**it);
        newResult.push_back('\n');
        results.emplace_back(std::move(newResult));
    }
    return results;
}

std::vector<std::string> UserDataMgr::BuildNAroundUser(User* user, const size_t num) {
    auto it = std::find(userSortedAmount.begin(), userSortedAmount.end(), user);
    if(it == userSortedAmount.end())
        return {};

    return std::vector<std::string>();
}

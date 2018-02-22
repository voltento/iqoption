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
        if(isStopped.load())
            return;

        if (!dataProvider->Next(rawCommand)) {
            static constexpr std::chrono::milliseconds timeout(10);
            while(!dataProvider->WaitNoEmpty(timeout)) {
                if(isStopped.load())
                    return;
            }
        } else {
            std::tuple<Command, User::Id, std::list<std::string>> cmdArg = ParseCommand(rawCommand);
            switch (std::get<0>(cmdArg)) {
                case Command::CONNECTED: {
                    ConnectUser(std::get<1>(cmdArg));
                }
                break;
                case Command::DISCONNECTED: {
                    DisconnectUser(std::get<1>(cmdArg));
                }
                break;
                case Command::DEAL_WON: {
                    const std::string &timeRaw = *std::get<2>(cmdArg).begin();
                    const std::string &amount = *std::next(std::get<2>(cmdArg).begin());
                    UserDealWon(std::get<1>(cmdArg), timeRaw, amount);
                }
                break;
                case Command::REGISTRED: {
                    RegistrateUser(std::get<1>(cmdArg), std::move(*std::get<2>(cmdArg).begin()));
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
    } else if (commandNameRaw == "user_connected") {
        if (!args.empty())
            LogCantParseCommand(0, args.size());
        else
            command = Command::CONNECTED;
    } else if (commandNameRaw == "user_disconnected") {
        if (!args.empty())
            LogCantParseCommand(0, args.size());
        else
            command = Command::DISCONNECTED;
    } else if (commandNameRaw == "user_deal_won") {
        if (args.size() != 2)
            LogCantParseCommand(2, args.size());
        else
            command = Command::DEAL_WON;
    }

    return std::make_tuple(command, userId, args);
}

void UserDataMgr::ProcCommand(const std::pair<UserDataMgr::Command, std::list<std::string>> &cmdArgs) {
    switch (cmdArgs.first) {
        case Command::DISCONNECTED : {

        }
            break;
        case Command::REGISTRED: {

        }
            break;
        case Command::CONNECTED: {

        }
            break;
        case Command::DEAL_WON: {

        }
            break;
        case Command::INVALID: {

        }
            break;
    }
}

void UserDataMgr::ConnectUser(User::Id userId) {
    std::lock_guard<std::mutex> guard(usersMutex);
    connectedUsers.erase(userId);
}

void UserDataMgr::DisconnectUser(User::Id userId) {
    std::lock_guard<std::mutex> guard(usersMutex);
    connectedUsers.insert(userId);
}

void UserDataMgr::RegistrateUser(User::Id userId, std::string &&name) {
    std::lock_guard<std::mutex> guard(usersMutex);
    auto it = users.find(userId);
    if (it != users.end()) {
        std::cerr << "Try register for already registered user. UserId: '" << userId << "'" << std::endl;
        return;
    }
    users[userId] = User{userId, std::move(name)};
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
        amount = std::stoll(amountRaw);
    }
    catch (const std::invalid_argument &ex) {
        std::cerr << "Can't parse use id from : '" << amountRaw << "'. Reason: '" << ex.what() << "'" << std::endl;
        return;
    }

    it->second.wonAmount += amount;
    // TODO: resort data!
}

void UserDataMgr::Stop() {
    isStopped.store(true);
}

#pragma once

#include <nlohmann/json.hpp>
#include "globals.hpp"
#include "network.hpp"

using nlohmann::json;

namespace lichess
{
    struct Game
    {
        std::string id;
        std::string name;
        std::string fen;
        bool isEngineWhite;
    };

    struct Challenge
    {
        std::string id;
        std::string name;
    };
    template <typename T>
    struct MUIList : public std::vector<T>
    {
        uint16_t get_cnt(void *data)
        {
            return this->size();
        }
        const char *get_str(void *data, uint16_t index)
        {
            if ((this->size() == 0) || (index > this->size() - 1))
                return "\0";
            return (*this)[index].name.c_str();
        }
    };

    MUIList<Game> games{};
    MUIList<Challenge> challenges{};

    uint16_t challenges_get_cnt(void *data) { return challenges.get_cnt(data); }
    const char *challenges_get_str(void *data, uint16_t index) { return challenges.get_str(data, index); }

    uint16_t games_get_cnt(void *data) { return games.get_cnt(data); }
    const char *games_get_str(void *data, uint16_t index) { return games.get_str(data, index); }

    uint16_t friends_get_cnt(void *data)
    {
        return accounts[account_selection].friends.size();
    }
    const char *friends_get_str(void *data, uint16_t index)
    {
        if (index > accounts[account_selection].friends.size() - 1)
            return "\0";
        return accounts[account_selection].friends[index].c_str();
    }

    void fetch_challenges()
    {
        challenges.clear();
        json res = apiGet("challenge");
        for (auto &challenge : res["in"])
        {
            challenges.push_back(
                Challenge{
                    challenge["id"],
                    challenge["challenger"]["id"].get<std::string>(),
                });
        }
    }

    void fetch_games()
    {
        games.clear();
        json res = apiGet("account/playing");
        for (auto &game : res["nowPlaying"])
        {
            games.push_back(
                Game{
                    game["gameId"],
                    game["opponent"]["username"].get<std::string>(),
                    game["fen"],
                    game["color"].get<std::string>() != "white", // Для engine наоборот
                });
        }
    }
}
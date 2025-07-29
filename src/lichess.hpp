#pragma once

#include <string>
#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "globals.hpp"

using nlohmann::json;
using std::string;

namespace lichess
{
    struct Game
    {
        // Game(string id, string opponent, string fen, bool isEngineWhite) : id(id), opponent(opponent), isEngineWhite(isEngineWhite), fen(fen) {}
        string id;
        string name;
        string fen;
        bool isEngineWhite;
    };

    struct Challenge
    {
        string id;
        string name;
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

    static const string baseApi = "https://lichess.org/api/";

    MUIList<Game> games{};
    MUIList<Challenge> challenges{};

    uint16_t challenges_get_cnt(void *data) { return challenges.get_cnt(data); }
    const char *challenges_get_str(void *data, uint16_t index) { return challenges.get_str(data, index); }

    uint16_t games_get_cnt(void *data) { return games.get_cnt(data); }
    const char *games_get_str(void *data, uint16_t index) { return games.get_str(data, index); }

    uint16_t friends_get_cnt(void *data)
    {
        return accounts[accountSelection].friends.size();
    }
    const char *friends_get_str(void *data, uint16_t index)
    {
        if (index > accounts[accountSelection].friends.size() - 1)
            return "\0";
        return accounts[accountSelection].friends[index].c_str();
    }

    auto getAuthStr()
    {
        return "Bearer " + accounts[accountSelection].authToken;
    }

    json apiPost(string path)
    {
        string url = baseApi + path;

        // Отправляем POST-запрос с заголовком авторизации
        cpr::Response response = cpr::Post(
            cpr::Url{url},
            cpr::Header{
                {"Authorization", getAuthStr()},
                // {"Content-Type", "application/json"},
            });

        if (response.status_code == 200)
        {
            return json::parse(response.text);
        }
        else
        {
            std::cerr << "Response: " << response.text << std::endl;
            return "err";
        }
    }

    json apiGet(string path)
    {
        string url = baseApi + path;

        cpr::Response response = cpr::Get(
            cpr::Url{url},
            cpr::Header{{"Authorization", getAuthStr()}});

        std::cout << "Response: " << response.text << std::endl;
        if (response.status_code == 200)
        {
            return json::parse(response.text);
        }
        else
        {
            return "err";
        }
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
                    challenge["challenger"]["id"].get<string>(),
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
                    game["opponent"]["username"].get<string>(),
                    game["fen"],
                    game["color"].get<string>() != "white", // Для engine наоборот
                });
        }
    }
}
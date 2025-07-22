#include <string>
#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using std::string;
using json = nlohmann::json;

namespace lichess
{
    static const string baseApi = "https://lichess.org/api/";
    static string authTokens[] = {"lip_7Rm30B9zcuMCwoRZ6mrB"};
    uint8_t accountId = 0;

    std::vector<std::vector<std::string>> challenges{};
    std::vector<std::vector<std::string>> games{};

    json apiPost(string path)
    {
        string url = baseApi + path;

        // Отправляем POST-запрос с заголовком авторизации
        cpr::Response response = cpr::Post(
            cpr::Url{url},
            cpr::Header{
                {"Authorization", "Bearer " + authTokens[accountId]},
                {"Content-Type", "application/json"}});

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
            cpr::Header{{"Authorization", "Bearer lip_7Rm30B9zcuMCwoRZ6mrB"}});
        // cpr::Header{{"Authorization", "Bearer " + authTokens[accountId]}});

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
        json res = apiGet("challenge");
        for (auto &challenge : res["in"])
        {
            challenges.push_back({challenge["id"],
                                  std::string("A") + challenge["challenger"]["id"].get<std::string>()});
        }
    }

    void fetch_games()
    {
        json res = apiGet("account/playing");
        for (auto &game : res["nowPlaying"])
        {
            std::cout << game["opponent"]["id"];
            games.push_back({game["gameId"],
                             std::string("A") + game["opponent"]["username"].get<std::string>()});
        }
        std::cout << games[0][1];
    }

    uint16_t challenges_get_cnt(void *data)
    {
        return challenges.size(); /* number of menu entries */
    }
    const char *challenges_get_str(void *data, uint16_t index)
    {
        if (index > challenges.size() - 1)
            return "\0";

        return challenges[index][1].c_str();
    }

    uint16_t games_get_cnt(void *data)
    {
        return games.size(); /* number of menu entries */
    }
    const char *games_get_str(void *data, uint16_t index)
    {
        if (index > games.size() - 1)
            return "\0";

        return games[index][1].c_str();
    }

}
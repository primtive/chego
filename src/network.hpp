#pragma once

#include "globals.hpp"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <string>
#include <iostream>

using nlohmann::json;
using std::string;

static const string baseApi = "https://lichess.org/api/";

auto getAuthStr()
{
    return "Bearer " + accounts[account_selection].authToken;
}

json apiPost(string path, cpr::Payload params)
{
    string url = baseApi + path;

    // Отправляем POST-запрос с заголовком авторизации
    cpr::Response response = cpr::Post(
        cpr::Url{url},
        params,
        cpr::Header{
            {"Authorization", getAuthStr()}});

    // 200 or 201
    return json::parse(response.text);
}

json apiPost(string path)
{
    return apiPost(path, cpr::Payload{});
}

json apiGet(string path)
{
    string url = baseApi + path;

    cpr::Response response = cpr::Get(
        cpr::Url{url},
        cpr::Header{
            {"Authorization", getAuthStr()},
        });

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
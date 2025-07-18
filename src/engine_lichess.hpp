#pragma once
#include "engine.hpp"
#include "chess.hpp"
#include <string>
#include <iostream>
#include <cpr/cpr.h>

class EngineLichess : public Engine
{
private:
    std::string authToken = "lip_7Rm30B9zcuMCwoRZ6mrB";
    std::string gameId = "FGnXNuFtnOPR";

    bool sendMoveToLichess(const std::string &moveUci)
    {
        // Формируем URL запроса
        std::string url = "https://lichess.org/api/board/game/" + gameId + "/move/" + moveUci;

        // Отправляем POST-запрос с заголовком авторизации
        cpr::Response response = cpr::Post(
            cpr::Url{url},
            cpr::Header{
                {"Authorization", "Bearer " + authToken},
                {"Content-Type", "application/json"}});

        // Проверяем результат
        if (response.status_code == 200)
        {
            std::cout << "Move " << moveUci << " sent successfully!" << std::endl;
            return true;
        }
        else
        {
            std::cerr << "Error sending move. Status code: " << response.status_code
                      << "\nResponse: " << response.text << std::endl;
            return false;
        }
    }
    void makeEngineMove() override
    {
    }

public:
    void makeMove(chess::Move move) override
    {
        sendMoveToLichess(chess::uci::moveToUci(move));
    }
};
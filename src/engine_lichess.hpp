#pragma once
#include "engine.hpp"
#include "chess.hpp"
#include "lichess.hpp"
#include <string>
#include <cpr/cpr.h>
#include <chrono>
#include <thread>
#include <iostream>

using namespace std::chrono_literals;

bool write(const std::string_view &data, intptr_t);
class EngineLichess;
static EngineLichess *engine;

class EngineLichess : public Engine
{
private:
    std::string gameId;

    void stream_lichess_game()
    {

        cpr::Session session;
        session.SetUrl(cpr::Url{"https://lichess.org/api/board/game/stream/" + gameId});
        session.SetHeader(cpr::Header{
            {"Accept", "application/x-ndjson"},
            {"Authorization", lichess::getAuthStr()} // Замените на реальный токен
        });
        session.SetWriteCallback(cpr::WriteCallback{write});
        cpr::Response response = session.Get();

        if (response.status_code != 200)
        {
            std::cerr << "Stream ended with status: " << response.status_code << std::endl;
            if (response.error)
            {
                std::cerr << "Error message: " << response.error.message << std::endl;
            }
        }
    }

public:
    EngineLichess(std::string gameId, chess::Color color) : gameId(gameId), Engine(color)
    {
        engine = this;
    }
    void makeMove(chess::Move move) override
    {
        lichess::apiPost("board/game/" + gameId + "/move/" + chess::uci::moveToUci(move));
    }
    void start() override
    {
        stream_lichess_game();
    }
};

std::string buffer_;
bool write(const std::string_view &data, intptr_t)
{
    buffer_.append(data);

    size_t pos = 0;
    while ((pos = buffer_.find('\n')) != std::string::npos)
    {
        if (pos == std::string::npos)
            break;

        std::string line = buffer_.substr(0, pos);
        buffer_.erase(0, pos + 1);

        if (!line.empty())
        {
            try
            {
                json message = json::parse(line);
                if (!message.contains("moves"))
                    return true;
                std::string str = message["moves"].get<std::string>();
                uint8_t length = static_cast<int>(std::count(str.begin(), str.end(), ' ')) + 1;
                std::cout << length << " abc " << (length % 2 == 0) << ((length % 2 == 0) == (engine->color == chess::Color::WHITE)) << std::endl;
                if ((length % 2 != 0) == (engine->color == chess::Color::WHITE))
                {
                    std::string move = str.substr(str.size() - 4);
                    std::cout << "engine moves " << move << std::endl;
                    engine->moveHandler(move);
                }
                else
                {
                    std::cout << "player moves\n";
                }
                std::cout << line << std::endl;
            }
            catch (const json::parse_error &e)
            {
                std::cerr << "JSON parse error: " << e.what() << std::endl;
                std::cerr << "Error parsing line: " << line << std::endl;
            }
        }
    }
    return true;
}
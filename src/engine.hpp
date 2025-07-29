// engine.hpp
#pragma once
#include "chess.hpp"

class Engine
{
protected:
public:
    chess::Color color;

    Engine() = default;
    Engine(chess::Color color) : color(color) {}
    std::function<void(std::string &)> moveHandler;
    void setMoveHandler(std::function<void(const std::string &)> handler)
    {
        moveHandler = handler;
    }
    virtual void makeMove(chess::Move move) = 0;
    virtual void start() = 0;
};
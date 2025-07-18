// engine.hpp
#pragma once
#include "chess.hpp"

class Engine
{
protected:
    chess::Color color = chess::Color::BLACK; // Черный по умолчанию
    std::function<void(const chess::Move &)> moveHandler;
    virtual void makeEngineMove() = 0;

public:
    Engine() = default;
    Engine(chess::Color color) : color(color) {}
    void setMoveHandler(std::function<void(const chess::Move &)> handler)
    {
        moveHandler = handler;
    }
    virtual void makeMove(chess::Move move) = 0;
};
#pragma once

#include "matrix_sfml.hpp"
#include "sensors_sfml.hpp"
#include "chess_board.hpp"
#include "chess_game.hpp"
#include "engine_lichess.hpp"

class Controller
{

public:
    std::unique_ptr<SFMLSensors> sensors;
    std::unique_ptr<SFMLMatrix> matrix;
    std::unique_ptr<ChessBoard> chessboard;
    std::unique_ptr<ChessGame> game;

    Controller() {}
    void init()
    {
        std::cout << "created chessboard" << std::endl;
        sensors = std::make_unique<SFMLSensors>(8);
        matrix = std::make_unique<SFMLMatrix>();

        chessboard = std::make_unique<ChessBoard>(*matrix);
        // chessboard->initWithAnim();
        chessboard->init();
    }
    void initChess()
    {
        game = std::make_unique<ChessGame>(*chessboard,
                                           sensors->state);
        if (game->checkPosition())
            startChess();
    }
    void initLichess(lichess::Game game_)
    {
        chess::Color engineColor = game_.isEngineWhite ? chess::Color::WHITE : chess::Color::BLACK;
        std::unique_ptr<Engine> engine = std::make_unique<EngineLichess>(game_.id, engineColor);
        game = std::make_unique<ChessGame>(*chessboard,
                                           sensors->state,
                                           engineColor,
                                           std::move(engine),
                                           game_.fen);
        if (game->checkPosition())
            startChess();
    }
    void startChess()
    {
        game->start();
        sensors->setHandler(game.get());
    }
};
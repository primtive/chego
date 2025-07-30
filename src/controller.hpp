#pragma once

#include "chess_board.hpp"
#include "chess_game.hpp"
#include "engine_lichess.hpp"
#include "engine_random.hpp"
#include "lichess.hpp"
#include "sensors_sfml.hpp"
#include "matrix_sfml.hpp"
#include "debug_window.hpp"

DebugWindow window;

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
        sensors = std::make_unique<SFMLSensors>(window, DebugWindow::SENSORS_SIZE);
        matrix = std::make_unique<SFMLMatrix>(window, DebugWindow::MATRIX_SIZE);

        window.setMatrix(matrix.get());
        window.setSensors(sensors.get());

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
    void initOfflineBot()
    {
        chess::Color engineColor = chess::Color::WHITE;
        std::string fen = chess::constants::STARTPOS;
        std::unique_ptr<Engine> engine = std::make_unique<EngineRandom>(engineColor, fen);
        game = std::make_unique<ChessGame>(*chessboard,
                                           sensors->state,
                                           engineColor,
                                           std::move(engine),
                                           fen);
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
    void resetChess()
    {
        game.reset();
        sensors->setHandler(nullptr);
        is_game_active = false;
    }
};
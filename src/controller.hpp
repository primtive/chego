#include "menu.hpp"
#include "matrix_sfml.hpp"
#include "sensors_sfml.hpp"
#include "chess_board.hpp"
#include "go_board.hpp"
#include "chess_game.hpp"
#include "engine_lichess.hpp"

enum class BoardType
{
    GoBoard,
    ChessBoard
};

class Controller
{
    BoardType getBoardType()
    {
        return BoardType::ChessBoard;
    }

public:
    // Menu menu;
    std::unique_ptr<Menu> menu;
    std::unique_ptr<SFMLSensors> sensors;
    std::unique_ptr<SFMLMatrix> matrix;
    std::unique_ptr<ChessBoard> chessboard;
    std::unique_ptr<ChessGame> game;
    EngineLichess engine;
    // GoBoard goboard;

    Controller() {}
    void init()
    {
        switch (getBoardType())
        {
        case BoardType::ChessBoard:
            std::cout << "created chessboard" << std::endl;
            sensors = std::make_unique<SFMLSensors>(8);
            matrix = std::make_unique<SFMLMatrix>(16);

            setenv("SDL_VIDEODRIVER", "wayland", 1);
            menu = std::make_unique<Menu>();

            chessboard = std::make_unique<ChessBoard>(*matrix);
            chessboard->initWithAnim();
            break;
        case BoardType::GoBoard:
            std::cout << "goboard" << std::endl;
            break;
        }
    }
    void startChess()
    {
        game = std::make_unique<ChessGame>(*chessboard,
                                           sensors->state,
                                           PlayerType::HUMAN,
                                           PlayerType::HUMAN,
                                           engine);
        sensors->setHandler(game.get());
    }
};
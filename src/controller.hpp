#include "menu.hpp"
#include "matrix_sfml.hpp"
#include "sensors_sfml.hpp"
#include "chess_board.hpp"
#include "go_board.hpp"
#include "chess_game.hpp"

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
    Menu menu;
    std::unique_ptr<SFMLSensors> sensors;
    std::unique_ptr<SFMLMatrix> matrix;
    std::unique_ptr<ChessBoard> chessboard;
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

            chessboard = std::make_unique<ChessBoard>(*matrix);
            chessboard->display();
            chessboard->draw();
            break;
        case BoardType::GoBoard:
            std::cout << "goboard" << std::endl;
            break;
        }
    }
};
#include "game.hpp"
#include "chess_game.hpp"
#include "hall_matrix_sfml.hpp"
#include "engine_lichess.hpp"
#include "engine_random.hpp"

class GameController
{
private:
    SFMLHallMatrix &hallMatrix;
    ChessBoard &chessboard;
    std::unique_ptr<ChessGame> game;
    EngineLichess engine;
    // EngineRandom engine;

public:
    GameController(SFMLHallMatrix &hallMatrix,
                   ChessBoard &chessboard)
        : hallMatrix(hallMatrix),
          chessboard(chessboard)
    {
        reset();
    }
    void reset()
    {

        game = std::make_unique<ChessGame>(chessboard,
                                           hallMatrix.matrixState,
                                           PlayerType::HUMAN,
                                           PlayerType::HUMAN,
                                           engine);
        hallMatrix.setHandler(game.get());
    }
};
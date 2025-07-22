#include "game.hpp"
#include "chess_game.hpp"
#include "sensors_sfml.hpp"
#include "engine_lichess.hpp"
#include "engine_random.hpp"

class GameController
{
private:
    SFMLSensors &sensors;
    ChessBoard &chessboard;
    std::unique_ptr<ChessGame> game;
    EngineLichess engine;
    // EngineRandom engine;

public:
    GameController(SFMLSensors &sensors,
                   ChessBoard &chessboard)
        : sensors(sensors),
          chessboard(chessboard)
    {
        reset();
    }
    void reset()
    {

        game = std::make_unique<ChessGame>(chessboard,
                                           sensors.state,
                                           PlayerType::HUMAN,
                                           PlayerType::HUMAN,
                                           engine);
        sensors.setHandler(game.get());
    }
};
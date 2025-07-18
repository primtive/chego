#include "matrix_sfml.hpp"
#include "hall_matrix_sfml.hpp"
#include "chess_board.hpp"
#include "go_board.hpp"
#include "chess_game.hpp"
#include "game_controller.hpp"
#include <SFML/Graphics.hpp>

int main()
{
    SFMLHallMatrix hallMatrix;
    SFMLMatrix matrix;

    ChessBoard chessboard(matrix);
    GoBoard goboard(matrix);

    chessboard.display();
    chessboard.draw();

    GameController gameController(hallMatrix, chessboard);

    while (matrix.isWindowOpen() && hallMatrix.isWindowOpen())
    {
        hallMatrix.poll();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
        {
            chessboard.displayWithAnim();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            goboard.displayWithAnim();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
        {
            gameController.reset();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
        {
            matrix.fill(Color::Black);
            matrix.draw();
        }
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T))
        // {
        //     game.highlightPieces();
        // }
    }

    return 0;
}
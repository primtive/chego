#include "matrix_sfml.hpp"
#include "hall_matrix_sfml.hpp"
#include "board_chess.hpp"
#include "board_go.hpp"
#include "logic_chess.hpp"
#include <SFML/Graphics.hpp>

int main()
{
    SFMLHallMatrix hallMatrix;
    SFMLMatrix matrix;

    ChessBoard chessboard(matrix);
    GoBoard goboard(matrix);

    ChessLogic chessLogic(chessboard, hallMatrix.hallMatrix);

    chessboard.display();
    chessboard.draw();

    hallMatrix.setHandler(&chessLogic);

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
            chessLogic.initStartPosition();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
        {
            matrix.fill(Color::Black);
            matrix.draw();
        }
    }

    return 0;
}
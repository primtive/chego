#include <memory>
#include <iostream>

#include "menu.hpp"
#include "matrix_sfml.hpp"
#include "sensors_sfml.hpp"
#include "chess_board.hpp"
#include "chess_game.hpp"
// #include "controller.hpp"
#include <SFML/Graphics.hpp>

int main()
{
    setenv("SDL_VIDEODRIVER", "x11", 1);
    Menu menu;
    SFMLSensors sensors(8);
    SFMLMatrix matrix(16);

    ChessBoard chessboard(matrix);
    chessboard.display();
    chessboard.draw();
    while (sensors.window.isOpen() && matrix.window.isOpen())
    {
        sensors.poll();

        while (const std::optional event = matrix.window.pollEvent())
        {
            if (!event->is<sf::Event::KeyPressed>())
                continue;
            switch (event->getIf<sf::Event::KeyPressed>()->scancode)
            {
            case sf::Keyboard::Scancode::Numpad8:
                std::cout << "up" << std::endl;
                menu.prev();
                break;
            case sf::Keyboard::Scancode::Numpad5:
                std::cout << "select" << std::endl;
                menu.select();
                break;
            case sf::Keyboard::Scancode::Numpad2:
                std::cout << "down" << std::endl;
                menu.next();
                break;
            default:
                // Unhandled scancode, do nothing
                break;
            }
        }
    }

    // Controller ctr;
    // ctr.init();

    // while (ctr.matrix->window.isOpen() && ctr.sensors->window.isOpen())
    // {
    //     ctr.sensors->poll();

    //     while (const std::optional event = ctr.matrix->window.pollEvent())
    //     {
    //         if (!event->is<sf::Event::KeyPressed>())
    //             continue;
    //         switch (event->getIf<sf::Event::KeyPressed>()->scancode)
    //         {
    //         case sf::Keyboard::Scancode::Q:
    //             ctr.chessboard->displayWithAnim();
    //             break;
    //         case sf::Keyboard::Scancode::Numpad8:
    //             std::cout << "up" << std::endl;
    //             ctr.menu.prev();
    //             break;
    //         case sf::Keyboard::Scancode::Numpad5:
    //             std::cout << "select" << std::endl;
    //             ctr.menu.select();
    //             break;
    //         case sf::Keyboard::Scancode::Numpad2:
    //             std::cout << "down" << std::endl;
    //             ctr.menu.next();
    //             break;
    //         default:
    //             // Unhandled scancode, do nothing
    //             break;
    //         }
    //     }
    // }

    return 0;
}
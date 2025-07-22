#include <memory>
#include <iostream>

#include "controller.hpp"
#include <SFML/Graphics.hpp>

int main()
{
    Controller ctr;
    ctr.init();

    while (ctr.matrix->window.isOpen() && ctr.sensors->window.isOpen())
    {
        ctr.sensors->poll();

        while (const std::optional event = ctr.matrix->window.pollEvent())
        {
            if (!event->is<sf::Event::KeyPressed>())
                continue;
            switch (event->getIf<sf::Event::KeyPressed>()->scancode)
            {
            case sf::Keyboard::Scancode::Q:
                ctr.chessboard->displayWithAnim();
                break;
            case sf::Keyboard::Scancode::Numpad8:
                std::cout << "up" << std::endl;
                ctr.menu->prev();
                break;
            case sf::Keyboard::Scancode::Numpad5:
                std::cout << "select" << std::endl;
                ctr.menu->select();
                break;
            case sf::Keyboard::Scancode::Numpad2:
                std::cout << "down" << std::endl;
                ctr.menu->next();
                break;
            default:
                // Unhandled scancode, do nothing
                break;
            }
        }
    }

    return 0;
}
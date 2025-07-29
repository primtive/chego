#include <memory>
#include <iostream>

#include "controller.hpp"
#include <SFML/Graphics.hpp>
#include "menu.hpp"

int main()
{
    controller_ptr = std::make_shared<Controller>();
    controller_ptr->init();

    setenv("SDL_VIDEODRIVER", "wayland", 1);
    std::unique_ptr<Menu> menu;
    menu = std::make_unique<Menu>();

    while (controller_ptr->matrix->window.isOpen() && controller_ptr->sensors->window.isOpen())
    {
        controller_ptr->sensors->poll();

        while (const std::optional event = controller_ptr->matrix->window.pollEvent())
        {
            if (!event->is<sf::Event::KeyPressed>())
                continue;
            switch (event->getIf<sf::Event::KeyPressed>()->scancode)
            {
            case sf::Keyboard::Scancode::Q:
                statusBar |= BOARD_TYPE_BIT;
                menu->draw();
                break;
            case sf::Keyboard::Scancode::W:
                controller_ptr->initChess();
                break;
            case sf::Keyboard::Scancode::E:
                controller_ptr->game->checkPosition();
                break;
            case sf::Keyboard::Scancode::Numpad8:
                std::cout << "up" << std::endl;
                menu->prev();
                break;
            case sf::Keyboard::Scancode::Numpad5:
                std::cout << "select" << std::endl;
                menu->select();
                break;
            case sf::Keyboard::Scancode::Numpad2:
                std::cout << "down" << std::endl;
                menu->next();
                break;
            default:
                // Unhandled scancode, do nothing
                break;
            }
        }
    }

    return 0;
}
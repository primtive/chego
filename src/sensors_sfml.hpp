#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <array>
#include "sensors_handler.hpp"

using sensors_t = std::vector<std::vector<bool>>;

class SFMLSensors
{
private:
    static const int HALL_PIXEL_SIZE = 50; // Размер клетки
    static const int HALL_SPACING = 2;
    static const sf::Font font;

public:
    sf::RenderWindow window;
    sensors_t state;
    uint8_t size;

    SFMLSensors(uint8_t size) : size(size)
    {
        uint16_t window_size = size * (HALL_PIXEL_SIZE + HALL_SPACING) - HALL_SPACING; // Размер окна
        window = sf::RenderWindow(
            sf::VideoMode({window_size, window_size}),
            "Debug panel",
            sf::Style::Close);
        state = sensors_t(
            size,
            std::vector<bool>(size));
        for (int y = 0; y < size; ++y)
        {
            for (int x = 0; x < size; ++x)
            {
                if (y < 2 || y > 5)
                {
                    state[y][x] = true; // Изначально датчики в этих строках включены
                }
                else
                {
                    state[y][x] = false; // Изначально датчики в этих строках выключены
                }
            }
        }
        display();
    }
    void display()
    {
        for (int blockY = 0; blockY < size; ++blockY)
        {
            for (int blockX = 0; blockX < size; ++blockX)
            {
                // Позиция блока
                int x = blockX * (HALL_PIXEL_SIZE + HALL_SPACING);
                int y = blockY * (HALL_PIXEL_SIZE + HALL_SPACING);

                sf::RectangleShape cell(sf::Vector2f(HALL_PIXEL_SIZE, HALL_PIXEL_SIZE));
                cell.setPosition({static_cast<float>(x), static_cast<float>(y)});
                cell.setFillColor(state[blockY][blockX] ? sf::Color::White : sf::Color::Black);
                window.draw(cell);
            }
        }
        for (int i = 0; i < size; ++i)
        {
            // ranks
            sf::Text text(font);
            text.setString((char)(i + 'a'));
            text.setCharacterSize(20);
            text.setFillColor(sf::Color::Red);
            text.setPosition({static_cast<float>(i * (HALL_PIXEL_SIZE + HALL_SPACING) + HALL_PIXEL_SIZE / 2),
                              0.0f});
            window.draw(text);

            // files
            text.setString(std::to_string(i + 1));
            text.setCharacterSize(20);
            text.setFillColor(sf::Color::Red);
            text.setPosition({0.0f,
                              static_cast<float>(i * (HALL_PIXEL_SIZE + HALL_SPACING) + HALL_PIXEL_SIZE / 2)});
            window.draw(text);
        }
        window.display();
    }
    void poll()
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            // Обработка кликов мыши
            if (const auto *mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                std::cout << "event" << std::endl;
                if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                {
                    int mouseX = sf::Mouse::getPosition(window).x;
                    int mouseY = sf::Mouse::getPosition(window).y;

                    // Преобразование координат мыши в координаты матрицы
                    int blockX = mouseX / (HALL_PIXEL_SIZE + HALL_SPACING);
                    int blockY = mouseY / (HALL_PIXEL_SIZE + HALL_SPACING);

                    if (blockX < size &&
                        blockY < size)
                    {

                        int x = blockX;
                        int y = blockY;

                        if (x < size && y < size)
                        {
                            // Переключение состояния светодиода
                            state[y][x] = !state[y][x];
                            triggerSensor(x, y, state[y][x]);
                            display();
                        }
                    }
                }
            }
        }
    }
    void setHandler(ISensorsHandler *handler)
    {
        m_handler = handler;
    }
    void triggerSensor(uint8_t x, uint8_t y, bool state)
    {
        if (m_handler)
        {
            m_handler->handle(x, y, state);
        }
    }

private:
    ISensorsHandler *m_handler = nullptr;
};

const sf::Font SFMLSensors::font = sf::Font("resources/arial.ttf");
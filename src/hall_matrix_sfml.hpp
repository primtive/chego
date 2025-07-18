#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <array>
#include "hall_handler.hpp"

const int HALL_PIXEL_SIZE = 50;                                                                  // Размер клетки
const int HALL_SPACING = 2;                                                                      // Промежуток между блоками
const int HALL_WINDOW_SIZE = HALL_MATRIX_SIZE * (HALL_PIXEL_SIZE + HALL_SPACING) - HALL_SPACING; // Размер окна

class SFMLHallMatrix
{
    sf::RenderWindow window;

public:
    std::vector<std::vector<bool>> matrixState;

    SFMLHallMatrix();

    void poll();
    bool isWindowOpen();

    void setHandler(IHallHandler *handler)
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
    IHallHandler *m_handler = nullptr;
};

SFMLHallMatrix::SFMLHallMatrix()
{
    window = sf::RenderWindow(
        sf::VideoMode({HALL_WINDOW_SIZE, HALL_WINDOW_SIZE}),
        "Debug panel",
        sf::Style::Close);
    matrixState = std::vector<std::vector<bool>>(
        HALL_MATRIX_SIZE,
        std::vector<bool>(HALL_MATRIX_SIZE));
    for (int y = 0; y < HALL_MATRIX_SIZE; ++y)
    {
        for (int x = 0; x < HALL_MATRIX_SIZE; ++x)
        {
            if (y < 2 || y > 5)
            {
                matrixState[y][x] = true; // Изначально датчики в этих строках включены
            }
            else
            {
                matrixState[y][x] = false; // Изначально датчики в этих строках выключены
            }
        }
    }
}

void SFMLHallMatrix::poll()
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();

        // Обработка кликов мыши
        if (const auto *mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseButtonPressed->button == sf::Mouse::Button::Left)
            {
                int mouseX = sf::Mouse::getPosition(window).x;
                int mouseY = sf::Mouse::getPosition(window).y;

                // Преобразование координат мыши в координаты матрицы
                int blockX = mouseX / (HALL_PIXEL_SIZE + HALL_SPACING);
                int blockY = mouseY / (HALL_PIXEL_SIZE + HALL_SPACING);

                if (blockX < HALL_MATRIX_SIZE &&
                    blockY < HALL_MATRIX_SIZE)
                {

                    int x = blockX;
                    int y = blockY;

                    if (x < HALL_MATRIX_SIZE && y < HALL_MATRIX_SIZE)
                    {
                        // Переключение состояния светодиода
                        matrixState[y][x] = !matrixState[y][x];
                        triggerSensor(x, y, matrixState[y][x]);
                    }
                }
            }
        }
    }

    for (int blockY = 0; blockY < HALL_MATRIX_SIZE; ++blockY)
    {
        for (int blockX = 0; blockX < HALL_MATRIX_SIZE; ++blockX)
        {
            // Позиция блока
            int x = blockX * (HALL_PIXEL_SIZE + HALL_SPACING);
            int y = blockY * (HALL_PIXEL_SIZE + HALL_SPACING);

            sf::RectangleShape cell(sf::Vector2f(HALL_PIXEL_SIZE, HALL_PIXEL_SIZE));
            cell.setPosition({static_cast<float>(x), static_cast<float>(y)});
            cell.setFillColor(matrixState[blockY][blockX] ? sf::Color::White : sf::Color::Black);
            window.draw(cell);
        }
    }
    for (int i = 0; i < HALL_MATRIX_SIZE; ++i)
    {
        // ranks
        sf::Font font("resources/arial.ttf");
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

bool SFMLHallMatrix::isWindowOpen()
{
    return window.isOpen();
}
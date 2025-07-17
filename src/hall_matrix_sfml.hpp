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
    std::vector<std::vector<bool>> hallMatrix;

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
    hallMatrix = std::vector<std::vector<bool>>(
        HALL_MATRIX_SIZE,
        std::vector<bool>(HALL_MATRIX_SIZE));
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
                        hallMatrix[y][x] = !hallMatrix[y][x];
                        triggerSensor(x, y, hallMatrix[y][x]);
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
            cell.setFillColor(hallMatrix[blockY][blockX] ? sf::Color::White : sf::Color::Black);
            window.draw(cell);
        }
    }
    window.display();
}

bool SFMLHallMatrix::isWindowOpen()
{
    return window.isOpen();
}
#include <SFML/Graphics.hpp>
#include "matrix.hpp"
#include <cmath>
#include <vector>
#include <array>
#include <chrono>
#include <thread>
#include <iostream>

using namespace std::chrono_literals;

const int PIXEL_SIZE = 28; // Размер одного LED
const int SPACING = 2;     // Промежуток между блоками
const int WINDOW_SIZE = (MATRIX_SIZE / CELL_SIZE) * (PIXEL_SIZE * CELL_SIZE + SPACING) - SPACING;
const Color bgColor(15, 15, 25);

class SFMLMatrix : public IMatrix
{
    sf::RenderWindow window;
    std::vector<std::vector<sf::Color>> ledMatrix;

private:
public:
    SFMLMatrix();

    void setCellColor(uint8_t x, uint8_t y, Color color);
    void draw();
    void fill(Color color);

    void poll();
    bool isWindowOpen();
};

SFMLMatrix::SFMLMatrix()
{
    window = sf::RenderWindow(
        sf::VideoMode({WINDOW_SIZE, WINDOW_SIZE}),
        "LED Matrix 16x16",
        sf::Style::Close);
    ledMatrix = std::vector<std::vector<sf::Color>>(
        MATRIX_SIZE,
        std::vector<sf::Color>(MATRIX_SIZE));

    fill(Color::Black);

    draw();
}

void SFMLMatrix::draw()
{
    std::this_thread::sleep_for(30ms); // Эмуляция задержки для обновления экрана

    window.clear(sf::Color(bgColor.r, bgColor.g, bgColor.b));

    // Рисование блоков 2x2 без внутренних разделителей
    for (int blockY = 0; blockY < MATRIX_SIZE / CELL_SIZE; ++blockY)
    {
        for (int blockX = 0; blockX < MATRIX_SIZE / CELL_SIZE; ++blockX)
        {
            // Позиция блока
            int blockPosX = blockX * (CELL_SIZE * PIXEL_SIZE + SPACING);
            int blockPosY = blockY * (CELL_SIZE * PIXEL_SIZE + SPACING);

            // Отрисовка светодиодов внутри блока
            for (int inY = 0; inY < CELL_SIZE; ++inY)
            {
                for (int inX = 0; inX < CELL_SIZE; ++inX)
                {
                    int x = blockX * CELL_SIZE + inX;
                    int y = blockY * CELL_SIZE + inY;

                    sf::RectangleShape led(sf::Vector2f(PIXEL_SIZE, PIXEL_SIZE));
                    led.setPosition(sf::Vector2f(
                        blockPosX + inX * PIXEL_SIZE,
                        blockPosY + inY * PIXEL_SIZE));
                    led.setFillColor(ledMatrix[y][x]);

                    // Скругление углов
                    led.setOutlineThickness(0);
                    window.draw(led);
                }
            }
        }
    }

    for (int i = 0; i < MATRIX_SIZE / CELL_SIZE; ++i)
    {
        // ranks
        sf::Font font("resources/arial.ttf");
        sf::Text text(font);
        text.setString((char)(i + 'a'));
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::Red);
        text.setPosition({static_cast<float>(i * (PIXEL_SIZE * CELL_SIZE + SPACING) + PIXEL_SIZE / 2),
                          0.0f});
        window.draw(text);

        // files
        text.setString(std::to_string(i + 1));
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::Red);
        text.setPosition({0.0f,
                          static_cast<float>(i * (PIXEL_SIZE * CELL_SIZE + SPACING) + PIXEL_SIZE / 2)});
        window.draw(text);
    }
    window.display();
}

void SFMLMatrix::setCellColor(uint8_t x, uint8_t y, Color color)
{
    ledMatrix[y][x] = sf::Color(color.r, color.g, color.b);
}

void SFMLMatrix::fill(Color color)
{
    for (uint8_t x = 0; x < MATRIX_SIZE; ++x)
    {
        for (uint8_t y = 0; y < MATRIX_SIZE; ++y)
        {
            setCellColor(x, y, color);
        }
    }
}

void SFMLMatrix::poll()
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
                int blockX = mouseX / (CELL_SIZE * PIXEL_SIZE + SPACING);
                int blockY = mouseY / (CELL_SIZE * PIXEL_SIZE + SPACING);

                int inBlockX = (mouseX - blockX * (CELL_SIZE * PIXEL_SIZE + SPACING)) / PIXEL_SIZE;
                int inBlockY = (mouseY - blockY * (CELL_SIZE * PIXEL_SIZE + SPACING)) / PIXEL_SIZE;

                if (blockX < MATRIX_SIZE / CELL_SIZE &&
                    blockY < MATRIX_SIZE / CELL_SIZE &&
                    inBlockX < CELL_SIZE &&
                    inBlockY < CELL_SIZE)
                {

                    int x = blockX * CELL_SIZE + inBlockX;
                    int y = blockY * CELL_SIZE + inBlockY;

                    if (x < MATRIX_SIZE && y < MATRIX_SIZE)
                    {
                        // Переключение состояния светодиода
                        setCellColor(x, y, Color::White);
                        draw();
                    }
                }
            }
        }
    }
}

bool SFMLMatrix::isWindowOpen()
{
    return window.isOpen();
}
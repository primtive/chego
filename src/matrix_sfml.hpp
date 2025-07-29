#include <SFML/Graphics.hpp>
#include "matrix.hpp"
#include <cmath>
#include <vector>
#include <array>
#include <chrono>
#include <thread>
#include <iostream>

using namespace std::chrono_literals;

class SFMLMatrix : public IMatrix
{
private:
    leds_t leds;
    static constexpr int PIXEL_SIZE = 28; // Размер одного LED
    static constexpr int SPACING = 2;     // Промежуток между блоками
    static constexpr int CELL_SIZE = 2;
    static constexpr uint16_t WINDOW_SIZE = (size / CELL_SIZE) * (PIXEL_SIZE * CELL_SIZE + SPACING) - SPACING;
    static const Color bgColor;

public:
    sf::RenderWindow window;

    SFMLMatrix()
    {
        window = sf::RenderWindow(
            sf::VideoMode({WINDOW_SIZE, WINDOW_SIZE}),
            "LED Matrix",
            sf::Style::Close);
        leds = leds_t(size, std::vector<Color>(size, Color::Black));
        // display();
    }

    void setCellColor(uint8_t x, uint8_t y, Color color);
    void display();
    void displayWithAnim(leds_t _leds);
    void fill(Color color);

    void poll();
};

void SFMLMatrix::display()
{
    window.clear(sf::Color(bgColor.r, bgColor.g, bgColor.b));

    // Рисование блоков 2x2 без внутренних разделителей
    for (int blockY = 0; blockY < size / CELL_SIZE; ++blockY)
    {
        for (int blockX = 0; blockX < size / CELL_SIZE; ++blockX)
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
                    led.setFillColor(leds[y][x].toSFMLColor());

                    // Скругление углов
                    led.setOutlineThickness(0);
                    window.draw(led);
                }
            }
        }
    }

    for (int i = 0; i < size / CELL_SIZE; ++i)
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

void SFMLMatrix::displayWithAnim(leds_t _leds)
{
    for (float r = 0; r <= size; r += 0.3)
    {
        for (int y = 0; y < size; y++)
        {
            for (int x = 0; x < size; x++)
            {
                // Расчет расстояния от центра
                float distance = sqrt(pow(x - center, 2) + pow(y - center, 2));

                // Если диод внутри круга с затуханием на границе
                if (r - distance < 1.0 && distance <= r)
                {
                    setCellColor(x, y, _leds[y][x]);
                }
            }
        }
        std::this_thread::sleep_for(30ms); // Эмуляция задержки для обновления экрана
        display();
    }
}

void SFMLMatrix::setCellColor(uint8_t x, uint8_t y, Color color)
{
    leds[y][x] = color;
}

void SFMLMatrix::fill(Color color)
{

    for (uint8_t x = 0; x < size; ++x)
    {
        for (uint8_t y = 0; y < size; ++y)
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

                if (blockX < size / CELL_SIZE &&
                    blockY < size / CELL_SIZE &&
                    inBlockX < CELL_SIZE &&
                    inBlockY < CELL_SIZE)
                {

                    int x = blockX * CELL_SIZE + inBlockX;
                    int y = blockY * CELL_SIZE + inBlockY;

                    if (x < size && y < size)
                    {
                        // Переключение состояния светодиода
                        setCellColor(x, y, Color::White);
                        display();
                    }
                }
            }
        }
    }
}

const Color SFMLMatrix::bgColor = Color(15, 15, 25);
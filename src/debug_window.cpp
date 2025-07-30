#include <SFML/Graphics.hpp>
#include "sensors_sfml.hpp"
#include "matrix_sfml.hpp"
#include "globals.hpp"
#include "debug_window.hpp"

void DebugWindow::display()
{
    window.clear(sf::Color::Black);
    // matrix

    // Рисование блоков 2x2 без внутренних разделителей
    for (int blockY = 0; blockY < MATRIX_SIZE / CELL_SIZE; ++blockY)
    {
        for (int blockX = 0; blockX < MATRIX_SIZE / CELL_SIZE; ++blockX)
        {
            // Позиция блока
            int blockPosX = blockX * BLOCK_SIZE;
            int blockPosY = blockY * BLOCK_SIZE;

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
                    led.setFillColor(matrix->leds[y][x].toSFMLColor(brightness));

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
                          static_cast<float>(i * BLOCK_SIZE + PIXEL_SIZE / 2)});
        window.draw(text);
    }

    // sensors
    for (int blockY = 0; blockY < SENSORS_SIZE; ++blockY)
    {
        for (int blockX = 0; blockX < SENSORS_SIZE; ++blockX)
        {
            // Позиция блока
            int x = blockX * BLOCK_SIZE + OFFSET;
            int y = blockY * BLOCK_SIZE;

            sf::RectangleShape cell(sf::Vector2f(PIXEL_SIZE * CELL_SIZE, PIXEL_SIZE * CELL_SIZE));
            cell.setPosition({static_cast<float>(x), static_cast<float>(y)});
            cell.setFillColor(sensors->state[blockY][blockX] ? sf::Color::White : sf::Color::Black);
            window.draw(cell);
        }
    }
    for (int i = 0; i < SENSORS_SIZE; ++i)
    {
        // ranks
        sf::Text text(font);
        text.setString((char)(i + 'a'));
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::Red);
        text.setPosition({static_cast<float>(i * BLOCK_SIZE + PIXEL_SIZE / 2 + OFFSET),
                          0.0f});
        window.draw(text);
    }
    window.display();
}
std::optional<sf::Event> DebugWindow::pollEvent()
{
    const std::optional<sf::Event> event = window.pollEvent();

    if (event)
    {
        if (event->is<sf::Event::Closed>())
            window.close();

        // Обработка кликов мыши
        if (const auto *mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseButtonPressed->button == sf::Mouse::Button::Left)
            {
                int mouseX = sf::Mouse::getPosition(window).x - OFFSET;
                int mouseY = sf::Mouse::getPosition(window).y;

                // Преобразование координат мыши в координаты матрицы
                int blockX = mouseX / (PIXEL_SIZE * CELL_SIZE + SPACING);
                int blockY = mouseY / (PIXEL_SIZE * CELL_SIZE + SPACING);

                if (blockX < SENSORS_SIZE &&
                    blockY < SENSORS_SIZE &&
                    blockX >= 0)
                {

                    int x = blockX;
                    int y = blockY;

                    if (x < SENSORS_SIZE && y < SENSORS_SIZE)
                    {
                        // Переключение состояния светодиода
                        sensors->state[y][x] = !sensors->state[y][x];
                        sensors->triggerSensor(x, y, sensors->state[y][x]);
                        display();
                    }
                }
            }
        }
        return event;
    }
    return std::nullopt;
}

const sf::Font DebugWindow::font = sf::Font("resources/arial.ttf");
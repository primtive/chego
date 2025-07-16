#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <array>

const int MATRIX_SIZE = 16; // Размер матрицы 16x16
const int BLOCK_SIZE = 2;   // Размер блока 2x2
const int PIXEL_SIZE = 28;  // Размер одного LED
const int SPACING = 2;      // Промежуток между блоками
const int WINDOW_SIZE = (MATRIX_SIZE / BLOCK_SIZE) * (PIXEL_SIZE * BLOCK_SIZE + SPACING) - SPACING;

int main()
{
    // Создание окна
    sf::RenderWindow window(
        sf::VideoMode({WINDOW_SIZE, WINDOW_SIZE}),
        "LED Matrix 16x16",
        sf::Style::Close);

    // Инициализация матрицы светодиодов
    std::vector<std::vector<sf::Color>> ledMatrix(
        MATRIX_SIZE,
        std::vector<sf::Color>(MATRIX_SIZE));

    // Цвета
    const sf::Color bgColor(15, 15, 25);
    const sf::Color ledOff(25, 25, 35);   // Черный
    const sf::Color ledOn(210, 210, 210); // Белый

    for (auto &row : ledMatrix)
    {
        std::fill(row.begin(), row.end(), ledOff);
    }

    // Основной цикл
    while (window.isOpen())
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
                    int blockX = mouseX / (BLOCK_SIZE * PIXEL_SIZE + SPACING);
                    int blockY = mouseY / (BLOCK_SIZE * PIXEL_SIZE + SPACING);

                    int inBlockX = (mouseX - blockX * (BLOCK_SIZE * PIXEL_SIZE + SPACING)) / PIXEL_SIZE;
                    int inBlockY = (mouseY - blockY * (BLOCK_SIZE * PIXEL_SIZE + SPACING)) / PIXEL_SIZE;

                    if (blockX < MATRIX_SIZE / BLOCK_SIZE &&
                        blockY < MATRIX_SIZE / BLOCK_SIZE &&
                        inBlockX < BLOCK_SIZE &&
                        inBlockY < BLOCK_SIZE)
                    {

                        int x = blockX * BLOCK_SIZE + inBlockX;
                        int y = blockY * BLOCK_SIZE + inBlockY;

                        if (x < MATRIX_SIZE && y < MATRIX_SIZE)
                        {
                            // Переключение состояния светодиода
                            ledMatrix[y][x] = ledOn;
                        }
                    }
                }
            }
        }

        // Отрисовка
        window.clear(bgColor);

        // Рисование блоков 2x2 без внутренних разделителей
        for (int blockY = 0; blockY < MATRIX_SIZE / BLOCK_SIZE; ++blockY)
        {
            for (int blockX = 0; blockX < MATRIX_SIZE / BLOCK_SIZE; ++blockX)
            {
                // Позиция блока
                int blockPosX = blockX * (BLOCK_SIZE * PIXEL_SIZE + SPACING);
                int blockPosY = blockY * (BLOCK_SIZE * PIXEL_SIZE + SPACING);

                // Отрисовка светодиодов внутри блока
                for (int inY = 0; inY < BLOCK_SIZE; ++inY)
                {
                    for (int inX = 0; inX < BLOCK_SIZE; ++inX)
                    {
                        int x = blockX * BLOCK_SIZE + inX;
                        int y = blockY * BLOCK_SIZE + inY;

                        sf::RectangleShape led(sf::Vector2f(PIXEL_SIZE, PIXEL_SIZE));
                        led.setPosition({blockPosX + inX * PIXEL_SIZE,
                                         blockPosY + inY * PIXEL_SIZE});
                        led.setFillColor(ledMatrix[y][x]);

                        // Скругление углов
                        led.setOutlineThickness(0);
                        window.draw(led);
                    }
                }
            }
        }
        // // Рисование сетки между блоками
        // for (int i = 0; i <= MATRIX_SIZE / BLOCK_SIZE; ++i)
        // {
        //     // Вертикальные линии
        //     sf::RectangleShape vline(sf::Vector2f(1, WINDOW_SIZE));
        //     vline.setPosition({i * (BLOCK_SIZE * PIXEL_SIZE + SPACING) - 2, 0});
        //     vline.setFillColor(gridColor);
        //     window.draw(vline);

        //     // Горизонтальные линии
        //     sf::RectangleShape hline(sf::Vector2f(WINDOW_SIZE, 1));
        //     hline.setPosition({0, i * (BLOCK_SIZE * PIXEL_SIZE + SPACING) - 1});
        //     hline.setFillColor(gridColor);
        //     window.draw(hline);
        // }

        window.display();
    }

    return 0;
}
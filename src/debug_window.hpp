#pragma once

#include <SFML/Graphics.hpp>

class SFMLSensors;
class SFMLMatrix;

class DebugWindow
{
public:
    static constexpr int MATRIX_SIZE = 16;
    static constexpr int SENSORS_SIZE = 8;

    void setSensors(SFMLSensors *s) { sensors = s; }
    void setMatrix(SFMLMatrix *m ) { matrix = m; }

    void display();
    std::optional<sf::Event> pollEvent();
    bool isOpen() { return window.isOpen(); }

    private:
        static const sf::Font font;
    static constexpr int PIXEL_SIZE = 25; // Размер клетки
    static constexpr int SPACING = 2;
    static constexpr int CELL_SIZE = 2;
    static constexpr int BLOCK_SIZE = CELL_SIZE * PIXEL_SIZE + SPACING;

    static constexpr int SCREEN_SIZE = BLOCK_SIZE * SENSORS_SIZE;
    static constexpr int SCREEN_SPACING = 100;
    static constexpr int OFFSET = SCREEN_SIZE + SCREEN_SPACING;

    sf::RenderWindow window = sf::RenderWindow(
        sf::VideoMode({SCREEN_SIZE + OFFSET, SCREEN_SIZE}),
        "Debug panel",
        sf::Style::Close);

    SFMLSensors *sensors;
    SFMLMatrix *matrix;
};
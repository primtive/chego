#include <cstdint>
#include <SFML/Graphics.hpp>

struct Color
{
public:
    uint8_t r, g, b;
    Color() : r(0), g(0), b(0) {}
    Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}

    sf::Color toSFMLColor()
    {
        return sf::Color(r, g, b);
    }
    sf::Color toSFMLColor(uint8_t brightness)
    {
        return sf::Color(r * brightness / 10, g * brightness / 10, b * brightness / 10);
    }
    bool operator==(Color a) const
    {
        if (a.r == r && a.g == g && a.b == b)
            return true;
        else
            return false;
    }

    static const Color White;
    static const Color Black;
    static const Color Go; // Wheat color for Go board
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color Orange;
    static const Color Purple;
};

const Color Color::White(255, 255, 255);
const Color Color::Black(0, 0, 0);
const Color Color::Go(245, 222, 179); // Wheat color for Go
const Color Color::Red(255, 0, 0);
const Color Color::Green(0, 255, 0); // Green color for highlighting
const Color Color::Blue(0, 0, 255);  // Blue color for highlighting
const Color Color::Orange(255, 165, 0);
const Color Color::Purple(128, 0, 128); // Purple color for highlighting
#include <cstdint>

class Color
{
public:
    uint8_t r, g, b;
    Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}

    static const Color White;
    static const Color Black;
    static const Color Go; // Wheat color for Go board
    static const Color Red;
    static const Color Green; 
};

const Color Color::White(255, 255, 255);
const Color Color::Black(0, 0, 0);
const Color Color::Go(245, 222, 179); // Wheat color for Go
const Color Color::Red(255, 0, 0);
const Color Color::Green(0, 255, 0); // Green color for highlighting
#pragma once
#include "matrix.hpp"

class DebugWindow;

class SFMLMatrix : public IMatrix
{
private:
    DebugWindow &window;

public:
    leds_t leds;

    SFMLMatrix(DebugWindow &window, uint8_t size);

    void setCellColor(uint8_t x, uint8_t y, Color color);
    void display();
    void displayWithAnim(leds_t _leds);
    void fill(Color color);
};
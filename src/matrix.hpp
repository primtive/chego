#pragma once
#include "color.hpp"

using leds_t = std::vector<std::vector<Color>>;

class IMatrix
{
protected:
public:
    uint8_t size;
    float center;

    IMatrix(uint8_t size) : size(size),
                            center((size - 1) / 2.0f) {}
    virtual ~IMatrix() = default;

    virtual void setCellColor(uint8_t x, uint8_t y, Color color) = 0;
    virtual void fill(Color color) = 0;

    virtual void draw() = 0;

    void drawWithAnim(leds_t leds)
    {
        for (uint8_t x = 0; x < size; x++)
        {
            for (uint8_t y = 0; y < size; y++)
            {
                setCellColor(x, y, leds[y][x]);
            }
        }
    }
};
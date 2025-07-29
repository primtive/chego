#pragma once
#include "color.hpp"
#include <cmath>

using leds_t = std::vector<std::vector<Color>>;

class IMatrix
{
protected:
public:
    static constexpr uint8_t size = 16;
    static constexpr float center = (size - 1) / 2.0f;

    virtual ~IMatrix() = default;

    virtual void setCellColor(uint8_t x, uint8_t y, Color color) = 0;
    virtual void fill(Color color) = 0;

    virtual void display() = 0;
    virtual void displayWithAnim(leds_t leds) = 0;
};
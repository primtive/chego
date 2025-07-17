#pragma once
#include <cstdint>

const int HALL_MATRIX_SIZE = 8; // Размер матрицы 16x16

class IHallHandler
{
public:
    virtual void handle(uint8_t x, uint8_t y, bool state) = 0;
    virtual ~IHallHandler() = default;
};
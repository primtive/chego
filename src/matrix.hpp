#pragma once
#include "color.hpp"

const int MATRIX_SIZE = 16; // Размер матрицы 16x16
const int CELL_SIZE = 2;    // Размер клетки 2x2
const float MATRIX_CENTER = (MATRIX_SIZE - 1) / 2.0;


class IMatrix
{
public:
    virtual ~IMatrix() = default;

    // Управление клетками
    virtual void setCellColor(uint8_t x, uint8_t y, Color color) = 0;
    virtual void fill(Color color) = 0;
    // virtual Color getCellColor(uint8_t x, uint8_t y) = 0;

    virtual void draw() = 0;
    // Управление состоянием матрицы
    // virtual void enable() = 0;
    // virtual void disable() = 0;
    // virtual bool isEnabled() const = 0;
};
#pragma once
#include "matrix.hpp"

class ChessBoard
{
    IMatrix &matrix;

public:
    ChessBoard(IMatrix &matrix);
    void display();
    void displayWithAnim();
    void draw() { matrix.draw(); }

    void setCell(uint8_t x, uint8_t y); // Clear cell
    void setCell(uint8_t x, uint8_t y, Color color);
};

ChessBoard::ChessBoard(IMatrix &matrix) : matrix(matrix)
{
}

void ChessBoard::display()
{
    for (uint8_t x = 0; x < MATRIX_SIZE; ++x)
    {
        for (uint8_t y = 0; y < MATRIX_SIZE; ++y)
        {
            if ((x / 2 + y / 2) % 2 == 0)
            {
                matrix.setCellColor(x, y, Color::White);
            }
            else
            {
                matrix.setCellColor(x, y, Color::Black);
            }
        }
    }
}

void ChessBoard::displayWithAnim()
{
    for (float r = 0; r <= MATRIX_SIZE; r += 0.3)
    {

        // Обход всех светодиодов
        for (int y = 0; y < MATRIX_SIZE; y++)
        {
            for (int x = 0; x < MATRIX_SIZE; x++)
            {
                // Расчет расстояния от центра
                float distance = sqrt(pow(x - MATRIX_CENTER, 2) + pow(y - MATRIX_CENTER, 2));

                // Если диод внутри круга с затуханием на границе
                if (distance <= r)
                {
                    if (r - distance < 1.0)
                    {
                        if ((x / 2 + y / 2) % 2 == 0)
                        {
                            // Установка цвета с плавным затуханием
                            matrix.setCellColor(x, y, Color::White);
                        }
                        else
                        {
                            matrix.setCellColor(x, y, Color::Black);
                        }
                    }
                }
            }
        }
        matrix.draw();
    }
}

void ChessBoard::setCell(uint8_t x, uint8_t y)
{
    const Color color = (x + y) % 2 == 0 ? Color::White : Color::Black;
    for (uint8_t inX = 0; inX < CELL_SIZE; ++inX)
    {
        for (uint8_t inY = 0; inY < CELL_SIZE; ++inY)
        {
            matrix.setCellColor(x * CELL_SIZE + inX, y * CELL_SIZE + inY, color);
        }
    }
}

void ChessBoard::setCell(uint8_t x, uint8_t y, Color color)
{
    for (uint8_t inX = 0; inX < CELL_SIZE; ++inX)
    {
        for (uint8_t inY = 0; inY < CELL_SIZE; ++inY)
        {
            matrix.setCellColor(x * CELL_SIZE + inX, y * CELL_SIZE + inY, color);
        }
    }
}
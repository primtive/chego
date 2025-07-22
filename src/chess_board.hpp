#pragma once
#include "matrix.hpp"
#include "chess.hpp"

class ChessBoard
{
    IMatrix &matrix;
    static const uint8_t CELL_SIZE = 2;

public:
    ChessBoard(IMatrix &matrix);
    void display();
    void displayWithAnim();
    void draw() { matrix.draw(); }

    void setCell(chess::Square square); // Clear cell
    void setCell(chess::Square square, Color color);
    void setCell(u_int8_t x, u_int8_t y); // Clear cell
    void setCell(u_int8_t x, u_int8_t y, Color color);
};

ChessBoard::ChessBoard(IMatrix &matrix) : matrix(matrix)
{
}

void ChessBoard::display()
{
    for (uint8_t x = 0; x < matrix.size; ++x)
    {
        for (uint8_t y = 0; y < matrix.size; ++y)
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
    for (float r = 0; r <= matrix.size; r += 0.3)
    {

        // Обход всех светодиодов
        for (int y = 0; y < matrix.size; y++)
        {
            for (int x = 0; x < matrix.size; x++)
            {
                // Расчет расстояния от центра
                float distance = sqrt(pow(x - matrix.center, 2) + pow(y - matrix.center, 2));

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

void ChessBoard::setCell(chess::Square square)
{
    setCell(static_cast<uint8_t>(square.file()), static_cast<uint8_t>(square.rank()));
}

void ChessBoard::setCell(chess::Square square, Color color)
{
    setCell(static_cast<uint8_t>(square.file()), static_cast<uint8_t>(square.rank()), color);
}

void ChessBoard::setCell(u_int8_t x, u_int8_t y)
{
    const Color color = (x + y) % 2 == 0 ? Color::White : Color::Black;
    setCell(x, y, color);
}

void ChessBoard::setCell(u_int8_t x, u_int8_t y, Color color)
{
    for (uint8_t inX = 0; inX < CELL_SIZE; ++inX)
    {
        for (uint8_t inY = 0; inY < CELL_SIZE; ++inY)
        {
            matrix.setCellColor(x * CELL_SIZE + inX, y * CELL_SIZE + inY, color);
        }
    }
}
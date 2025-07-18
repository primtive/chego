#include "matrix.hpp"

class GoBoard
{
    const sf::Color goColor = sf::Color(245, 222, 179);
    IMatrix &matrix;

public:
    GoBoard(IMatrix &matrix);
    void display();
    void displayWithAnim();
};

GoBoard::GoBoard(IMatrix &matrix) : matrix(matrix)
{
}

void GoBoard::display()
{
    matrix.fill(Color::Go);
    matrix.draw();
}

void GoBoard::displayWithAnim()
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
                        matrix.setCellColor(x, y, Color::Go);
                    }
                }
            }
        }
        matrix.draw();
    }
}
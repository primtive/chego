#include "matrix.hpp"

class GoBoard
{
    static const sf::Color goColor;
    IMatrix &matrix;

public:
    GoBoard(IMatrix &matrix) : matrix(matrix) {}
    void display();
    void displayWithAnim();
};
const sf::Color GoBoard::goColor = sf::Color(245, 222, 179);

void GoBoard::display()
{
    matrix.fill(Color::Go);
    matrix.draw();
}

void GoBoard::displayWithAnim()
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
                        matrix.setCellColor(x, y, Color::Go);
                    }
                }
            }
        }
        matrix.draw();
    }
}
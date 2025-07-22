#include "matrix.hpp"

class GoBoard
{
    static const sf::Color goColor;
    IMatrix &matrix;

public:
    GoBoard(IMatrix &matrix) : matrix(matrix) {}
    void init();
    void initWithAnim();
};
const sf::Color GoBoard::goColor = sf::Color(245, 222, 179);

void GoBoard::init()
{
    matrix.fill(Color::Go);
    matrix.display();
}

void GoBoard::initWithAnim()
{
    leds_t leds(matrix.size, std::vector<Color>(matrix.size, Color::Go));
    matrix.displayWithAnim(leds);
}
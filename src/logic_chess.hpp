#include "board_chess.hpp"
#include "hall_handler.hpp"

class ChessLogic : public IHallHandler
{
private:
    ChessBoard &board;
    std::vector<std::vector<bool>> &hallMatrix;

public:
    ChessLogic(ChessBoard &board, std::vector<std::vector<bool>> &hallMatrix) : board(board), hallMatrix(hallMatrix) {};
    void handle(uint8_t x, uint8_t y, bool state) override
    {
        // board.setCell(x, y, Color::Go);
        // board.draw();
    }
    void initStartPosition()
    {
        board.display();
        bool isOk = true;
        for (uint8_t x = 0; x < HALL_MATRIX_SIZE; ++x)
        {
            for (uint8_t y = 0; y < HALL_MATRIX_SIZE; ++y)
            {
                if (y < 2 || y > 5)
                {
                    if (!hallMatrix[y][x])
                    {
                        board.setCell(x, y, Color::Green);
                        isOk = false;
                    }
                }
                else
                {
                    if (hallMatrix[y][x])
                    {
                        board.setCell(x, y, Color::Red);
                        isOk = false;
                    }
                }
            }
        }
        board.draw();
        if (isOk)
        {
            
        }
    }
};
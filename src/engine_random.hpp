#include "engine.hpp"
#include <random>
#include <thread>

class EngineRandom : public Engine
{
private:
    std::mt19937 rng{std::random_device{}()};
    chess::Board board;

    void makeEngineMove() override
    {
        if (board.sideToMove() != color)
        {
            std::cout << "not my color" << std::endl;
            return;
        }
        chess::Movelist moves;
        chess::movegen::legalmoves(moves, board);
        if (moves.size() == 0)
            return;

        std::uniform_int_distribution<int> dist(0, moves.size() - 1);
        int index = dist(rng);
        if (moveHandler)
        {
            board.makeMove(moves[index]);
            moveHandler(moves[index]);
        }
    }

public:
    void makeMove(chess::Move move) override
    {
        board.makeMove(move);

        std::thread([this, move]()
                    {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                makeEngineMove(); })
            .detach();
    }
};
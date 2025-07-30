#include "engine.hpp"
#include <random>
#include <thread>

class EngineRandom : public Engine
{
private:
    std::mt19937 rng{std::random_device{}()};
    chess::Board board;

    void makeEngineMove()
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
            std::string move_uci = chess::uci::moveToUci(moves[index]);
            moveHandler(move_uci);
        }
    }

public:
    EngineRandom(chess::Color color, std::string fen) : Engine(color), board(fen) {}
    void makeMove(chess::Move move) override
    {
        board.makeMove(move);

        std::thread([this, move]()
                    {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                makeEngineMove(); })
            .detach();
    }
    void start() override {
        if (board.sideToMove() == color) {
            makeEngineMove();
        }
    }
};
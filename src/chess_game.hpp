#pragma once
#include "chess_board.hpp"
#include "game.hpp"
#include "chess.hpp"
#include "sensors_handler.hpp"
#include "engine.hpp"

enum class PlayerType
{
    HUMAN,
    ENGINE
};

class ChessGame : public IGame, public ISensorsHandler
{
private:
    ChessBoard &board;
    chess::Board game;
    sensors_t &sensorsState;
    PlayerType whitePlayerType;
    PlayerType blackPlayerType;
    Engine &engine;

    static constexpr uint8_t INCORRECT_SQUARES_BUFFER_SIZE = 8;

    chess::Movelist moves;
    std::array<chess::Square, INCORRECT_SQUARES_BUFFER_SIZE> incorrectSquares;
    chess::Square felledSquare;
    chess::Square selectedSquare;
    chess::Square selectedAttackedSquare;
    chess::Square checkSquare;

    static void printBoardFromFen(const std::string &fen)
    {
        std::vector<std::vector<char>> board(8, std::vector<char>(8, ' '));

        size_t fenIndex = 0;
        int row = 0;
        int col = 0;

        // Парсим часть FEN, описывающую расположение фигур
        while (fenIndex < fen.size() && fen[fenIndex] != ' ')
        {
            char c = fen[fenIndex];

            if (c == '/')
            {
                // Новая строка доски
                row++;
                col = 0;
            }
            else if (isdigit(c))
            {
                // Пустые клетки
                int empty = c - '0';
                for (int i = 0; i < empty; i++)
                {
                    if (col < 8)
                    {
                        board[row][col] = '.';
                        col++;
                    }
                }
            }
            else
            {
                // Фигура
                if (col < 8)
                {
                    board[row][col] = c;
                    col++;
                }
            }

            fenIndex++;
        }

        // Выводим доску
        std::cout << "  +-----------------+" << std::endl;
        for (int i = 0; i < 8; i++)
        {
            std::cout << 8 - i << " | ";
            for (int j = 0; j < 8; j++)
            {
                std::cout << board[i][j] << " ";
            }
            std::cout << "|" << std::endl;
        }
        std::cout << "  +-----------------+" << std::endl;
        std::cout << "    a b c d e f g h" << std::endl;
    }
    void setIncorrectSquare(const chess::Square square)
    {
        board.setCell(square, Color::Red);
        chess::Square *emptySquare;
        emptySquare = std::find(std::begin(incorrectSquares),
                                std::end(incorrectSquares),
                                chess::Square::NO_SQ);
        *emptySquare = square;
    }
    void clearPossibleMoves(chess::Square square)
    {
        for (const auto &move : moves)
        {
            if (move.from() == square)
            {
                board.setCell(move.to());
            }
        }
        board.setCell(square);
    }
    void highlightPossibleMoves(chess::Square square)
    {
        for (const auto &move : moves)
        {
            if (move.from() == square)
            {
                board.setCell(move.to(), Color::Blue);
            }
        }
        board.setCell(square, Color::Purple); // Выделяем выбранную фигуру
    }
    void processPieceMove(chess::Square square, bool state)
    {
        std::cout << game.sideToMove();
        for (chess::Square &sq : incorrectSquares)
        {
            if (square == sq) // Убираем выделение с неправильного хода, при возвращении или удалении фигуры
            {
                board.setCell(sq);
                sq = chess::Square::NO_SQ;
                return;
            }
        }

        if (((game.sideToMove() == chess::Color::WHITE && whitePlayerType == PlayerType::ENGINE) ||
             (game.sideToMove() == chess::Color::BLACK && blackPlayerType == PlayerType::ENGINE)))
            setIncorrectSquare(square);

        if (state)
        {
            if (selectedSquare == chess::Square::NO_SQ && selectedAttackedSquare == chess::Square::NO_SQ) // Поставили фигуру когда нет выбранной
            {
                setIncorrectSquare(square);
            }
            else if (selectedAttackedSquare == square) // Вернули на место атакованную фигуру
            {
                selectedAttackedSquare = chess::Square::NO_SQ;
                board.setCell(square);
                for (const auto &move : moves)
                {
                    if (move.to() == square)
                    {
                        board.setCell(move.from());
                    }
                }
            }
            else if (square == selectedSquare) // Снятие выделения с выбранной фигуры, поставленной на место
            {
                clearPossibleMoves(selectedSquare);
                selectedSquare = chess::Square::NO_SQ;
            }
            else
            {
                for (const auto &move : moves)
                {
                    if (move.from() == selectedSquare && move.to() == square) // Сделан ход
                    {
                        if (felledSquare != chess::Square::NO_SQ && felledSquare != move.to())
                        {
                            break;
                        }
                        if (felledSquare != chess::Square::NO_SQ)
                            felledSquare = chess::Square::NO_SQ; // Сбрасываем срубленную фигуру

                        game.makeMove(move);
                        if (game.sideToMove() == chess::Color::WHITE && whitePlayerType == PlayerType::ENGINE ||
                            game.sideToMove() == chess::Color::BLACK && blackPlayerType == PlayerType::ENGINE)
                        {
                            engine.makeMove(move);
                        }

                        clearPossibleMoves(selectedSquare);
                        selectedSquare = chess::Square::NO_SQ;
                        if (move.typeOf() == chess::Move::ENPASSANT) // Взятие на проходе
                        {
                            chess::Rank rank = move.to().rank() == chess::Rank::RANK_3 ? chess::Rank::RANK_4 : chess::Rank::RANK_5;
                            chess::Square enpassantSquare = chess::Square(move.to().file(), rank);
                            // chess::Square enpassantSquare = game.enpassantSq();
                            setIncorrectSquare(enpassantSquare);
                        }
                        chess::movegen::legalmoves(moves, game);
                        return;
                    }
                }
                setIncorrectSquare(square); // Если не нашли ход, то выделяем неправильный ход
            }
        }
        else
        {
            if (selectedSquare != chess::Square::NO_SQ || selectedAttackedSquare != chess::Square::NO_SQ) // Если фигура уже выбрана
            {
                for (const auto &move : moves)
                {
                    if (square == move.to() && move.from() == selectedSquare) // Убираем фигуру с доски, когда рубим
                    {
                        clearPossibleMoves(selectedSquare);
                        felledSquare = move.to();
                        board.setCell(felledSquare, Color::Purple);
                        return;
                    }
                    else if (square == move.from() && move.to() == selectedAttackedSquare) // Убираем нашу фигуру с доски, когда рубим
                    {
                        // board.setCell(square);
                        for (const auto &move : moves)
                        {
                            if (move.to() == selectedAttackedSquare)
                            {
                                board.setCell(move.from());
                            }
                        }
                        felledSquare = move.to();
                        board.setCell(felledSquare, Color::Purple);
                        selectedSquare = square;
                        selectedAttackedSquare = chess::Square::NO_SQ;
                        return;
                    }
                }
                setIncorrectSquare(square); // Выделение неправильного хода, если фигура уже выбрана
            }
            else
            {
                for (const auto &move : moves)
                {
                    if (square == move.from()) // Выбор фигуры и выделение возможных ходов
                    {
                        selectedSquare = square;
                        highlightPossibleMoves(square);
                        return;
                    }
                    else if (square == move.to()) // Если сначала убираем срубленную фигуру
                    {
                        selectedAttackedSquare = square;
                        board.setCell(square, Color::Blue);
                        for (const auto &move : moves)
                        {
                            if (move.to() == square)
                            {
                                board.setCell(move.from(), Color::Purple);
                            }
                        }
                        return;
                    }
                }
                // Выделение фигуры, которой нельзя ходить
                setIncorrectSquare(square);
            }
        }
    }
    void handleEngineMove(chess::Move move)
    {
        game.makeMove(move);
        chess::movegen::legalmoves(moves, game);
        setIncorrectSquare(move.from());
        setIncorrectSquare(move.to());
        board.display();
    }

public:
    ChessGame(ChessBoard &board,
              sensors_t &sensorsState,
              PlayerType whitePlayer, PlayerType blackPlayer,
              Engine &engine)
        : board(board),
          sensorsState(sensorsState),
          game(chess::constants::STARTPOS),
          whitePlayerType(whitePlayer),
          blackPlayerType(blackPlayer),
          engine(engine)
    {
        chess::movegen::legalmoves(moves, game);
        engine.setMoveHandler([this](const chess::Move &move)
                              {
                // Обработка хода от движка
                this->handleEngineMove(move); });
    };
    bool checkStartPos()
    {
        board.init();
        bool isOk = true;
        for (uint8_t x = 0; x < sensorsState.size(); ++x)
        {
            for (uint8_t y = 0; y < sensorsState.size(); ++y)
            {
                if (y < 2 || y > 5)
                {
                    if (!sensorsState[y][x])
                    {
                        board.setCell(x, y, Color::Green);
                        isOk = false;
                    }
                }
                else
                {
                    if (sensorsState[y][x])
                    {
                        board.setCell(x, y, Color::Red);
                        isOk = false;
                    }
                }
            }
        }
        board.display();
        return isOk;
    };
    void handle(uint8_t x, uint8_t y, bool state) override
    {
        chess::Square square = chess::Square(static_cast<chess::File>(x), static_cast<chess::Rank>(y));
        processPieceMove(square, state);

        chess::GameResult gameResult = game.isGameOver().second;
        if (gameResult != chess::GameResult::NONE)
        {
            Color whiteKingColor = Color::Orange;
            Color blackKingColor = Color::Orange;
            switch (gameResult)
            {
            case chess::GameResult::WIN:
                whiteKingColor = Color::Red;
                blackKingColor = Color::Green;
                break;
            case chess::GameResult::LOSE: // White wins
                whiteKingColor = Color::Green;
                blackKingColor = Color::Red;
                break;
            default:
                break;
            }
            board.setCell(game.kingSq(chess::Color::WHITE), whiteKingColor);
            board.setCell(game.kingSq(chess::Color::BLACK), blackKingColor);
            board.display();
            delete this;
            return;
        }
        if (game.inCheck()) // Проверка на шах
        {
            checkSquare = game.kingSq(game.sideToMove()); // Сохраняем клетку короля, если он под шахом
            board.setCell(checkSquare, Color::Orange);
        }
        else if (checkSquare != chess::Square::NO_SQ)
        {
            board.setCell(checkSquare);
            checkSquare = chess::Square::NO_SQ; // Сбрасываем клетку шаха
        }

        std::string fen = game.getFen();
        printBoardFromFen(fen);

        board.display();
    }
    void highlightPieces()
    {
        for (uint8_t x = 0; x < sensorsState.size(); ++x)
        {
            for (uint8_t y = 0; y < sensorsState.size(); ++y)
            {
                chess::Square square = chess::Square(static_cast<chess::File>(x), static_cast<chess::Rank>(y));
                chess::PieceType pieceType = game.at<chess::PieceType>(square);
                Color color = Color::Black;
                switch (static_cast<std::uint8_t>(pieceType))
                {
                case static_cast<std::uint8_t>(chess::PieceType::PAWN):
                    color = Color::Purple;
                    break;
                case static_cast<std::uint8_t>(chess::PieceType::KNIGHT):
                    color = Color::Red;
                    break;
                case static_cast<std::uint8_t>(chess::PieceType::BISHOP):
                    color = Color::Green;
                    break;
                case static_cast<std::uint8_t>(chess::PieceType::ROOK):
                    color = Color::Orange;
                    break;
                case static_cast<std::uint8_t>(chess::PieceType::QUEEN):
                    color = Color::Blue;
                    break;
                case static_cast<std::uint8_t>(chess::PieceType::KING):
                    color = Color::White;
                    break;
                }
                board.setCell(square, color);
            }
        }
        board.display();
    }
};
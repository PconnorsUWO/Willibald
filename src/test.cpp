#include "test.h"
#include "../include/chessboard.h"
#include "../include/controller.h"
#include "../include/evaluate.h"
#include "../include/fen.h"
#include "../include/chess.h"
#include "../include/move.h"
#include "../include/movegen.h"
#include "../include/logging.h"
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <cassert>

static Chessboard makeMove(const Chessboard& board, const Move& move, const int color)
{
    Chessboard new_board = board.Copy();
    const Controller controller(new_board);
    controller.MakeMove(move, color);
    return new_board;
}

static double NormalizeEvaluation(double score)
{
    return 1.0 / (1.0 + std::exp(-score / 400.0));
}

void Test::RunEvaluationTests()
{
    std::cout << "Running Evaluation Tests..." << std::endl;
    Chessboard board;
    Controller controller(board);
    std::string test_fen =
        "rn1qkbnr/ppp1pppp/8/3p4/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 0 3";
    controller.ParseFen(test_fen);
    Evaluator evaluator(Evaluator::SymmetricEvaluation);
    double score = evaluator.Evaluate(board);
    std::cout << "Evaluation score for test position: " << score << std::endl;
}

void Test::RunSearchTests()
{
    std::cout << "Search test placeholder: search function called." << std::endl;
}

void Test::RunRuntimeTests()
{
    std::cout << "Running Runtime Performance Tests..." << std::endl;
    // Runtime tests code goes here.
}

void Test::RunMoveTests()
{
    std::cout << "Running Controller::MakeMove tests..." << std::endl;

    // Test Case 1: Normal move (non-capture).
    {
        std::cout << "Test Case 1: Normal move (e2 -> e3)" << std::endl;
        Chessboard board;
        Controller controller(board);
        controller.ParseFen(Fen::START_POSITION);
        int source = 52;
        int target = 44;
        Move move(source, target, Chess::P, 0, 0, 0, 0, 0);
        controller.MakeMove(move, Chess::WHITE);
        assert(!board.GetPieceOccupancy(Chess::P).Test(source) &&
            "Source square not cleared for normal move");
        assert(board.GetPieceOccupancy(Chess::P).Test(target) &&
            "Target square not set for normal move");
        std::cout << "Test Case 1 passed." << std::endl;
    }

    // Test Case 2: Capture move.
    {
        std::cout << "Test Case 2: Capture move (e4 x d5)" << std::endl;
        std::string fen = "8/8/8/3p4/4P3/8/8/8 w - - 0 1";
        Chessboard board;
        Controller controller(board);
        controller.ParseFen(fen);
        int source = 36;
        int target = 27;
        Move move(source, target, Chess::P, 0, 1, 0, 0, 0);
        controller.MakeMove(move, Chess::WHITE);
        assert(!board.GetPieceOccupancy(Chess::P).Test(source) &&
            "Source square not cleared for capture move");
        assert(board.GetPieceOccupancy(Chess::P).Test(target) &&
            "Target square not set for capture move");
        assert(!board.GetPieceOccupancy(Chess::p).Test(target) &&
            "Captured black pawn not cleared");
        std::cout << "Test Case 2 passed." << std::endl;
    }

    // Test Case 3: En passant move.
    {
        std::cout << "Test Case 3: En passant capture" << std::endl;
        std::string fen = "8/8/8/3p4/4P3/8/8/8 w - d6 0 1";
        Chessboard board;
        Controller controller(board);
        controller.ParseFen(fen);
        int source = 36;
        int target = 19;
        Move move(source, target, Chess::P, 0, 1, 0, 1, 0);
        controller.MakeMove(move, Chess::WHITE);
        assert(!board.GetPieceOccupancy(Chess::P).Test(source) &&
            "Source square not cleared for en passant");
        assert(board.GetPieceOccupancy(Chess::P).Test(target) &&
            "Target square not set for en passant");
        assert(!board.GetPieceOccupancy(Chess::p).Test(27) &&
            "En passant capture did not remove black pawn");
        std::cout << "Test Case 3 passed." << std::endl;
    }

    // Test Case 4: Promotion move.
    {
        std::cout << "Test Case 4: Promotion move (e7 -> e8, promote to Queen)" << std::endl;
        std::string fen = "8/4P3/8/8/8/8/8/8 w - - 0 1";
        Chessboard board;
        Controller controller(board);
        controller.ParseFen(fen);
        int source = 12;
        int target = 4;
        Move move(source, target, Chess::P, Chess::Q, 0, 0, 0, 0);
        controller.MakeMove(move, Chess::WHITE);
        assert(!board.GetPieceOccupancy(Chess::P).Test(source) &&
            "Source square not cleared for promotion");
        assert(board.GetPieceOccupancy(Chess::Q).Test(target) &&
            "Promoted queen not placed at target square");
        assert(board.GetColorOccupancy(Chess::WHITE).Test(target) &&
            "White occupancy not updated for promotion");
        std::cout << "Test Case 4 passed." << std::endl;
    }

    std::cout << "All Controller::MakeMove tests passed." << std::endl;
}

void Test::RunMoveGenTests()
{
    std::cout << "Running MoveGen Tests..." << std::endl;

    Chessboard board;
    Controller controller(board);
    controller.ParseFen(Fen::START_POSITION);

    auto whiteMoves = MoveGen::GetMoves(board, Chess::WHITE);
    std::cout << "White Moves (" << whiteMoves.size() << "):" << std::endl;
    for (const Move& m : whiteMoves)
    {
        Log::PrintMoveEncoding(m);
    }

    auto blackMoves = MoveGen::GetMoves(board, Chess::BLACK);
    std::cout << "Black Moves (" << blackMoves.size() << "):" << std::endl;
    for (const Move& m : blackMoves)
    {
        Log::PrintMoveEncoding(m);
    }

    std::cout << "MoveGen Tests completed." << std::endl;
}

uint64_t Test::Perft(const Chessboard& board, int depth)
{
    if (depth == 0)
        return 1; // Count the current position

    uint64_t nodes = 0;
    std::vector<Move> moves = MoveGen::GetLegalMoves(board, board.GetSideToMove());

    if (depth == 1)
        return moves.size();

    for (const Move& move : moves)
    {
        Chessboard new_board = makeMove(board, move, board.GetSideToMove());
        nodes += Perft(new_board, depth - 1);
    }

    return nodes;
}
#include "../include/search.h"
#include "../include/movegen.h"
#include "../include/evaluate.h"
#include "../include/controller.h"
#include "../include/chessboard.h"
#include "../include/chess.h"
#include <limits>
#include <vector>
#include <iostream>

static Chessboard makeMove(const Chessboard& board, const Move& move, const int color)
{
    Chessboard new_board = board.Copy();
    const Controller controller(new_board);
    controller.MakeMove(move, color);
    return new_board;
}

SearchAlgorithm::SearchAlgorithm(int depth, void* threadpool)
    : depth(depth), threadpool(threadpool)
{
}

inline int OpponentColor(int color) { return (color == Chess::WHITE) ? Chess::BLACK : Chess::WHITE; }

// Within Recursive call cahin.
SearchResult negamax(const Chessboard& board, const int depth, Evaluator& evaluator, const int color)
{
    std::vector<Move> moves = MoveGen::GetMoves(board, color);

    if (depth == 0 || moves.empty())
    {
        double eval = evaluator.Evaluate(board);
        if (color == Chess::BLACK)
        {
            eval = -eval;
        }
        return SearchResult(Move(0, 0, 0, 0, 0, 0, 0, 0), eval);
    }

    double best = -std::numeric_limits<double>::infinity();
    Move best_move(0, 0, 0, 0, 0, 0, 0, 0);
    int opponentColor = OpponentColor(color);

    for (const Move& m : moves)
    {
        Chessboard new_board = makeMove(board, m, color);
        SearchResult result = negamax(new_board, depth - 1, evaluator, opponentColor);
        double score = -result.evaluation;
        if (score > best)
        {
            best = score;
            best_move = m;
        }
    }
    return SearchResult(best_move, best);
}

SearchAlgorithm::~SearchAlgorithm()
= default;

void SearchAlgorithm::setDepth(int newDepth)
{
    depth = newDepth;
}

int SearchAlgorithm::getDepth() const
{
    return depth;
}

void SearchAlgorithm::setThreadPool(void* newThreadPool)
{
    threadpool = newThreadPool;
}

void* SearchAlgorithm::getThreadPool() const
{
    return threadpool;
}

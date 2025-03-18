#pragma once

#include "../include/move.h"
#include "../include/chessboard.h"
#include "../include/evaluate.h"

struct SearchResult
{
    Move best_move;
    double evaluation;

    SearchResult() : best_move(0, 0, 0, 0, 0, 0, 0, 0), evaluation(0.0)
    {
    }

    SearchResult(const Move& m, const double eval) : best_move(m), evaluation(eval)
    {
    }
};

class SearchAlgorithm final
{
public:
    explicit SearchAlgorithm(int depth, void* threadpool = nullptr);
    virtual ~SearchAlgorithm();
    void setDepth(int newDepth);
    [[nodiscard]] int getDepth() const;
    void setThreadPool(void* newThreadPool);
    [[nodiscard]] void* getThreadPool() const;
    virtual Move minimaxAlphaBeta(const Chessboard& board) { return {0, 0, 0, 0, 0, 0, 0, 0}; }
    virtual Move bruteforceSearch(const Chessboard& board) { return {0, 0, 0, 0, 0, 0, 0, 0}; }

protected:
    int depth;
    void* threadpool;
};

SearchResult negamax(const Chessboard& board, int depth, Evaluator& evaluator, int color);

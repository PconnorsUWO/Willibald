#pragma once

#include "../include/move.hpp"
#include "../include/chess_board.hpp" 
#include "../include/evaluate.hpp"

struct SearchResult {
    Move best_move;
    double evaluation;
    SearchResult() : best_move(0, 0, 0, 0, 0, 0, 0, 0), evaluation(0.0) {}
    SearchResult(const Move &m, double eval) : best_move(m), evaluation(eval) {}
};

class SearchAlgorithm {
public:
    SearchAlgorithm(int depth, void* threadpool = nullptr);
    virtual ~SearchAlgorithm();
    void setDepth(int newDepth);
    int getDepth() const;
    void setThreadPool(void* newThreadPool);
    void* getThreadPool() const;
    virtual Move minimaxAlphaBeta(const ChessBoard& board) { return Move(0, 0, 0, 0, 0, 0, 0, 0); }
    virtual Move bruteforceSearch(const ChessBoard& board) { return Move(0, 0, 0, 0, 0, 0, 0, 0); }

protected:
    int depth;          
    void* threadpool;   
};

SearchResult negamax(const ChessBoard &board, int depth, Evaluator &evaluator, int color);
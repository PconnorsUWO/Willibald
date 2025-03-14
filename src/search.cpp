#include "../include/search.hpp"
#include "../include/movegen.hpp"
#include "../include/evaluate.hpp"
#include "../include/controller.hpp"
#include "../include/chess_board.hpp"
#include "../include/encodings.hpp"
#include <limits>
#include <vector>
#include <iostream>

static ChessBoard makeMove(const ChessBoard &board, Move move, int color) {
    ChessBoard new_board = board.Copy();
    Controller controller(new_board);
    controller.MakeMove(move, color);
    return new_board;
}

SearchAlgorithm::SearchAlgorithm(int depth, void* threadpool)
    : depth(depth), threadpool(threadpool)
{
}

inline int OpponentColor(int color) { return (color == ChessEncoding::WHITE) ? ChessEncoding::BLACK : ChessEncoding::WHITE; }
  
SearchResult negamax(const ChessBoard &board, int depth, Evaluator &evaluator, int color) {    
    std::vector<Move> moves = MoveGen::GetMoves(board, color);
    
    if (depth == 0 || moves.empty()) {
        double eval = evaluator.Evaluate(board);
        if (color == ChessEncoding::BLACK) {
            eval = -eval;
        }
        return SearchResult(Move(0, 0, 0, 0, 0, 0, 0, 0), eval);
    }
    
    double best = -std::numeric_limits<double>::infinity();
    Move best_move(0, 0, 0, 0, 0, 0, 0, 0);
    int opponentColor = OpponentColor(color);
    
    for (const Move &m : moves) {
        ChessBoard new_board = makeMove(board, m, color);
        SearchResult result = negamax(new_board, depth - 1, evaluator, opponentColor);
        double score = -result.evaluation;
        if (score > best) {
            best = score;
            best_move = m;
        }
    }
    return SearchResult(best_move, best);
}

SearchAlgorithm::~SearchAlgorithm() {
}

void SearchAlgorithm::setDepth(int newDepth) {
    depth = newDepth;
}

int SearchAlgorithm::getDepth() const {
    return depth;
}

void SearchAlgorithm::setThreadPool(void* newThreadPool) {
    threadpool = newThreadPool;
}

void* SearchAlgorithm::getThreadPool() const {
    return threadpool;
}

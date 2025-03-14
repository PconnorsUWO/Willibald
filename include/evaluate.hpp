#ifndef EVALUATE_HPP_
#define EVALUATE_HPP_

#include "chess_board.hpp"
#include <functional>

class Evaluator {
public:
    using EvalFunc = std::function<double(const ChessBoard&)>;

    explicit Evaluator(EvalFunc evaluator);
    void SetEvaluator(EvalFunc new_evaluator);
    double Evaluate(const ChessBoard& board) const;
    static double SymmetricEvaluation(const ChessBoard& board);

private:
    EvalFunc evaluator_;
};

#endif  // EVALUATE_HPP_

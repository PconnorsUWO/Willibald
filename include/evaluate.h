#pragma once

#include "chessboard.h"
#include <functional>

class Evaluator
{
public:
    using EvalFunc = std::function<double(const Chessboard&)>;

    explicit Evaluator(EvalFunc evaluator);
    void SetEvaluator(EvalFunc new_evaluator);
    [[nodiscard]] double Evaluate(const Chessboard& board) const;
    static double SymmetricEvaluation(const Chessboard& board);

private:
    EvalFunc evaluator_;
};

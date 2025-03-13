#ifndef EVALUATE_HPP_
#define EVALUATE_HPP_

#include "chess_board.hpp"
#include <functional>
#include <string>

namespace evaluate {

class Evaluator {
 public:
  using EvalFunc = std::function<double(const ChessBoard&)>;

  explicit Evaluator(EvalFunc evaluator);

  void SetEvaluator(EvalFunc new_evaluator);

  double Evaluate(const ChessBoard& board) const;

  static double SymmetricEvaluation(const ChessBoard& board);

  static double NormalizeEvaluation(double score);

  double RunTest(const std::string& csv_filename = "data.csv") const;

 private:
  EvalFunc evaluator_;
};

}  // namespace evaluate

#endif  // EVALUATE_HPP_

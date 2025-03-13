#include "evaluate.hpp"
#include "encodings.hpp"
#include "movegen.hpp"
#include "chess_board.hpp"
#include "controller.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

namespace {

// Count doubled pawns: for each file, any pawn beyond the first is doubled.
int CountDoubledPawns(const ChessBoard& board, bool white) {
  int file_counts[8] = {0};
  int pawn_piece = white ? ChessEncoding::P : ChessEncoding::p;
  auto pawns = board.GetPieceOccupancy(pawn_piece);
  while (pawns) {
    int square = pawns.PopLsbIndex();
    int file = square % 8;
    file_counts[file]++;
  }
  int doubled = 0;
  for (int f = 0; f < 8; f++) {
    if (file_counts[f] > 1)
      doubled += (file_counts[f] - 1);
  }
  return doubled;
}

// Count blocked pawns:
// For White, a pawn is blocked if the square immediately above (square - 8) is occupied.
// For Black, if the square immediately below (square + 8) is occupied.
int CountBlockedPawns(const ChessBoard& board, bool white) {
  int blocked = 0;
  int pawn_piece = white ? ChessEncoding::P : ChessEncoding::p;
  auto pawns = board.GetPieceOccupancy(pawn_piece);
  auto all_pieces = board.GetColorOccupancy(ChessEncoding::BOTH);
  while (pawns) {
    int square = pawns.PopLsbIndex();
    int target_square;
    if (white) {
      if (square >= 8) {  // not on the top rank
        target_square = square - 8;
        if (all_pieces.Test(target_square))
          blocked++;
      }
    } else {
      if (square < 56) {  // not on the bottom rank
        target_square = square + 8;
        if (all_pieces.Test(target_square))
          blocked++;
      }
    }
  }
  return blocked;
}

// Count isolated pawns:
// A pawn is isolated if there is no friendly pawn on the adjacent file(s).
int CountIsolatedPawns(const ChessBoard& board, bool white) {
  int file_counts[8] = {0};
  int pawn_piece = white ? ChessEncoding::P : ChessEncoding::p;
  auto pawn_copy = board.GetPieceOccupancy(pawn_piece);
  while (pawn_copy) {
    int square = pawn_copy.PopLsbIndex();
    int file = square % 8;
    file_counts[file]++;
  }
  int isolated = 0;
  auto pawns = board.GetPieceOccupancy(pawn_piece);
  while (pawns) {
    int square = pawns.PopLsbIndex();
    int file = square % 8;
    bool left_empty = (file == 0) || (file_counts[file - 1] == 0);
    bool right_empty = (file == 7) || (file_counts[file + 1] == 0);
    if (left_empty && right_empty)
      isolated++;
  }
  return isolated;
}

}  // namespace

namespace evaluate {

Evaluator::Evaluator(EvalFunc evaluator) : evaluator_(evaluator) {}

void Evaluator::SetEvaluator(EvalFunc new_evaluator) {
  evaluator_ = new_evaluator;
}

double Evaluator::Evaluate(const ChessBoard& board) const {
  return evaluator_(board);
}

double Evaluator::SymmetricEvaluation(const ChessBoard& board) {
  double score = 0.0;

  // --- Material Evaluation ---
  int white_kings   = board.GetPieceOccupancy(ChessEncoding::K).CountBits();
  int black_kings   = board.GetPieceOccupancy(ChessEncoding::k).CountBits();
  int white_queens  = board.GetPieceOccupancy(ChessEncoding::Q).CountBits();
  int black_queens  = board.GetPieceOccupancy(ChessEncoding::q).CountBits();
  int white_rooks   = board.GetPieceOccupancy(ChessEncoding::R).CountBits();
  int black_rooks   = board.GetPieceOccupancy(ChessEncoding::r).CountBits();
  int white_bishops = board.GetPieceOccupancy(ChessEncoding::B).CountBits();
  int black_bishops = board.GetPieceOccupancy(ChessEncoding::b).CountBits();
  int white_knights = board.GetPieceOccupancy(ChessEncoding::N).CountBits();
  int black_knights = board.GetPieceOccupancy(ChessEncoding::n).CountBits();
  int white_pawns   = board.GetPieceOccupancy(ChessEncoding::P).CountBits();
  int black_pawns   = board.GetPieceOccupancy(ChessEncoding::p).CountBits();

  score += 200.0 * (white_kings - black_kings);
  score +=   9.0 * (white_queens - black_queens);
  score +=   5.0 * (white_rooks - black_rooks);
  score +=   3.0 * ((white_bishops - black_bishops) +
                    (white_knights - black_knights));
  score +=   1.0 * (white_pawns - black_pawns);

  // --- Pawn Structure Evaluation ---
  int white_doubled  = CountDoubledPawns(board, true);
  int black_doubled  = CountDoubledPawns(board, false);
  int white_blocked  = CountBlockedPawns(board, true);
  int black_blocked  = CountBlockedPawns(board, false);
  int white_isolated = CountIsolatedPawns(board, true);
  int black_isolated = CountIsolatedPawns(board, false);

  double pawn_structure_diff = (white_doubled - black_doubled) +
                               (white_blocked - black_blocked) +
                               (white_isolated - black_isolated);
  score -= 0.5 * pawn_structure_diff;

  // --- Mobility Evaluation ---
  int white_mobility = MoveGen::GetMoves(board, ChessEncoding::WHITE).size();
  int black_mobility = MoveGen::GetMoves(board, ChessEncoding::BLACK).size();
  score += 0.1 * (white_mobility - black_mobility);

  return score;
}

double Evaluator::NormalizeEvaluation(double score) {
  return 1.0 / (1.0 + std::exp(-score / 400.0));
}

double Evaluator::RunTest(const std::string& csv_filename) const {
  std::ifstream file(csv_filename);
  if (!file.is_open()) {
    std::cerr << "Error opening CSV file: " << csv_filename << std::endl;
    return -1.0;
  }

  std::string line;
  std::getline(file, line);

  int count = 0;
  double mse = 0.0;
  ChessBoard board;
  Controller controller(board);

  while (std::getline(file, line)) {
    if (line.empty())
      continue;
    if (count >= 20000)
      break;

    std::istringstream ss(line);
    std::string fen, eval_str;
    if (std::getline(ss, fen, ',') && std::getline(ss, eval_str)) {
      if (eval_str[0] == '#')
        continue;
      double ref_eval = std::stod(eval_str);
      controller.ParseFen(fen);
      double raw_eval = evaluator_(board) * 100.0;
      double norm_raw = NormalizeEvaluation(raw_eval);
      double norm_ref = NormalizeEvaluation(ref_eval);
      double diff = norm_raw - norm_ref;
      mse += diff * diff;
      ++count;
      std::cout << "FEN: " << fen
                << " True Eval: " << ref_eval
                << " Estimated Eval: " << raw_eval << std::endl;
    }
  }

  if (count > 0)
    mse /= count;

  return mse;
}

}  // namespace evaluate

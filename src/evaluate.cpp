#include "../include/evaluate.h"
#include "../include/chess.h"
#include "../include/movegen.h"
#include "../include/chessboard.h"
#include <cmath>

static int CountDoubledPawns(const Chessboard& board, bool white)
{
  int file_counts[8] = {0};
  int pawn_piece = white ? Chess::P : Chess::p;
  auto pawns = board.GetPieceOccupancy(pawn_piece);
  while (pawns)
  {
    int square = pawns.PopLsbIndex();
    int file = square % 8;
    file_counts[file]++;
  }
  int doubled = 0;
  for (int f = 0; f < 8; f++)
  {
    if (file_counts[f] > 1)
    {
      doubled += (file_counts[f] - 1);
    }
  }
  return doubled;
}

static int CountBlockedPawns(const Chessboard& board, bool white)
{
  int blocked = 0;
  int pawn_piece = white ? Chess::P : Chess::p;
  auto pawns = board.GetPieceOccupancy(pawn_piece);
  auto all_pieces = board.GetColorOccupancy(Chess::BOTH);
  while (pawns)
  {
    int square = pawns.PopLsbIndex();
    int target_square;
    if (white)
    {
      if (square >= 8)
      {
        target_square = square - 8;
        if (all_pieces.Test(target_square))
        {
          blocked++;
        }
      }
    }
    else
    {
      if (square < 56)
      {
        target_square = square + 8;
        if (all_pieces.Test(target_square))
        {
          blocked++;
        }
      }
    }
  }
  return blocked;
}

static int CountIsolatedPawns(const Chessboard& board, bool white)
{
  int file_counts[8] = {0};
  int pawn_piece = white ? Chess::P : Chess::p;
  auto pawn_copy = board.GetPieceOccupancy(pawn_piece);
  while (pawn_copy)
  {
    int square = pawn_copy.PopLsbIndex();
    int file = square % 8;
    file_counts[file]++;
  }
  int isolated = 0;
  auto pawns = board.GetPieceOccupancy(pawn_piece);
  while (pawns)
  {
    int square = pawns.PopLsbIndex();
    int file = square % 8;
    bool left_empty = (file == 0) || (file_counts[file - 1] == 0);
    bool right_empty = (file == 7) || (file_counts[file + 1] == 0);
    if (left_empty && right_empty)
    {
      isolated++;
    }
  }
  return isolated;
}

Evaluator::Evaluator(EvalFunc evaluator) : evaluator_(evaluator)
{
}

void Evaluator::SetEvaluator(EvalFunc new_evaluator)
{
  evaluator_ = new_evaluator;
}

double Evaluator::Evaluate(const Chessboard& board) const
{
  return evaluator_(board);
}

double Evaluator::SymmetricEvaluation(const Chessboard& board)
{
  double score = 0.0;

  // --- Material Evaluation ---
  int white_kings = board.GetPieceOccupancy(Chess::K).CountBits();
  int black_kings = board.GetPieceOccupancy(Chess::k).CountBits();
  int white_queens = board.GetPieceOccupancy(Chess::Q).CountBits();
  int black_queens = board.GetPieceOccupancy(Chess::q).CountBits();
  int white_rooks = board.GetPieceOccupancy(Chess::R).CountBits();
  int black_rooks = board.GetPieceOccupancy(Chess::r).CountBits();
  int white_bishops = board.GetPieceOccupancy(Chess::B).CountBits();
  int black_bishops = board.GetPieceOccupancy(Chess::b).CountBits();
  int white_knights = board.GetPieceOccupancy(Chess::N).CountBits();
  int black_knights = board.GetPieceOccupancy(Chess::n).CountBits();
  int white_pawns = board.GetPieceOccupancy(Chess::P).CountBits();
  int black_pawns = board.GetPieceOccupancy(Chess::p).CountBits();

  score += 200.0 * (white_kings - black_kings);
  score += 9.0 * (white_queens - black_queens);
  score += 5.0 * (white_rooks - black_rooks);
  score += 3.0 * ((white_bishops - black_bishops) +
    (white_knights - black_knights));
  score += 1.0 * (white_pawns - black_pawns);

  // --- Pawn Structure Evaluation ---
  int white_doubled = CountDoubledPawns(board, true);
  int black_doubled = CountDoubledPawns(board, false);
  int white_blocked = CountBlockedPawns(board, true);
  int black_blocked = CountBlockedPawns(board, false);
  int white_isolated = CountIsolatedPawns(board, true);
  int black_isolated = CountIsolatedPawns(board, false);

  double pawn_structure_diff = (white_doubled - black_doubled) +
    (white_blocked - black_blocked) +
    (white_isolated - black_isolated);
  score -= 0.5 * pawn_structure_diff;

  // --- Mobility Evaluation ---
  int white_mobility = MoveGen::GetMoves(board, Chess::WHITE).size();
  int black_mobility = MoveGen::GetMoves(board, Chess::BLACK).size();
  score += 0.1 * (white_mobility - black_mobility);

  return score;
}

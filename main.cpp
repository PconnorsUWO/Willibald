//
// Created by Alexander King Perocho on 2025-03-10.
//

#include <iostream>
#include "include/logging.h"
#include "include/controller.h"
#include "include/chessboard.h"
#include "include/attack_mask.h"
#include "include/movegen.h"

int main()
{
  int color = 0;
  AttackMask::initialize();
  Chessboard chessboard;
  const Controller controller(chessboard);
  controller.ParseFen(Fen::KILLER_POSITION);
  Log::PrintChessboard(chessboard);

  // Log::PrintMovesEncoding(MoveGen::GetKingMoves(chessboard, 1));

  // std::cout << "P: " << MoveGen::GetPawnMoves(chessboard, 0).size() << std::endl;
  // std::cout << "p: " << MoveGen::GetPawnMoves(chessboard, 1).size() << std::endl;
  std::cout << "K: " << MoveGen::GetKingMoves(chessboard, 0).size() << std::endl;
  std::cout << "k: " << MoveGen::GetKingMoves(chessboard, 1).size() << std::endl;

  // Log::PrintMoves(chessboard, color);
  // Log::PrintAttackedSquares(chessboard, color);
  // std::vector<Move> moves = MoveGen::GetMoves(chessboard, color);
  // Log::printMovesEncoding(moves);
  // Log::PrintMoveEncoding(moves[4]);
  // controller.MakeMove(moves[4], color);
  // Log::PrintChessBoard(chessboard);
  return 0;
}

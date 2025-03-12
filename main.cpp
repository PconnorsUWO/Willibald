//
// Created by Alexander King Perocho on 2025-03-10.
//

#include "include/chessboard.h"
#include "include/attack_mask.h"

int main()
{
  AttackMask::initialize();
  const Chessboard board;
  board.GetColorOccupancy(2).Print();
  return 0;
}

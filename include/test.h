#pragma once
#include "chessboard.h"

class Test {
    public:
       static void RunEvaluationTests();
       static void RunSearchTests();
       static void RunRuntimeTests();
       static void RunMoveTests();
       static void RunMoveGenTests();
       static uint64_t Perft(const Chessboard& board, int depth);
   };
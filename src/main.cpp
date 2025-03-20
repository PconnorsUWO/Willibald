#include <iostream>
#include <vector>
#include "../include/attack_mask.h"
#include "../include/movegen.h"
#include "../include/logging.h"
#include "../include/fen.h"
#include "../include/chessboard.h"
#include "../include/controller.h"
#include "../include/evaluate.h"
#include "../include/test.h"
#include "../include/search.h"
#include "../include/chess.h"
#include "../include/stacktrace.h"

#ifdef _WIN32
#include <Windows.h>
#include <eh.h>  // For _set_se_translator
#endif

int main()
{
#ifdef _WIN32
    _set_se_translator(StackTrace::seTranslator);
#endif

    StackTrace::registerSignalHandler();
    AttackMask::initialize(); // Initialize attack masks

    Chessboard chessboard;
    Controller controller(chessboard);
    controller.ParseFen("kbK5/pp6/1P6/8/8/8/8/R7 w - - 0 1");
    Log::PrintChessboard(chessboard);
    Evaluator evaluator(Evaluator::SymmetricEvaluation);
    SearchResult result = negamax(chessboard, 5, evaluator, 0);
    Log::PrintMoveEncoding(result.best_move);
    std::cout << "Score: " << result.evaluation << std::endl;
    // Log Chess Board
    // Log::PrintChessboard(chessboard);
    // const Bitboard occupancy = chessboard.GetColorOccupancy(Chess::BOTH);
    // Log::PrintBitboard(occupancy);
    // Log Raw Attack Masks
    // for (int i = 0; i < 64; i++)
    // {
    //     std::cout << "square: " << i << std::endl;
    //     Log::PrintBitboard(
    //         AttackMask::GetAttackMask(
    //             AttackMask::BlackPawn, i));
    // }

    // Log Blocked Attack Masks
    // Log::PrintBitboard(
    //     AttackMask::GetAttackMask(
    //         AttackMask::Rook, Chess::a1, chessboard.GetColorOccupancy(Chess::BOTH)));

    // for (int square = 0; square < 64; square++)
    // {
    //     std::cout << "Queen attack mask for square " << square << " (" 
    //               << Chess::SQUARE_TO_ALGEBRAIC_NOTATION[square] << "):" << std::endl;
    //     Log::PrintBitboard(
    //         AttackMask::GetAttackMask(AttackMask::Queen, square, occupancy)
    //     );
    //     std::cout << std::endl;
    // }

}

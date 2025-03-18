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
    const Controller controller(chessboard);
    controller.ParseFen(Fen::FEN_2);
    Evaluator evaluator(Evaluator::SymmetricEvaluation);
    SearchResult result = negamax(chessboard, 2, evaluator, Chess::BLACK);

    Log::PrintMoveEncoding(result.best_move);
    std::cout << "Eval: " << result.evaluation << std::endl;

    return 0;
}

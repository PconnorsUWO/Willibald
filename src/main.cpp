#include <iostream>
#include <vector>
// #include "../include/mask.hpp"
#include "../include/attack_mask.hpp"
#include "../include/movegen.hpp"
#include "../include/logging.hpp"
#include "../include/FEN.hpp"
#include "../include/chess_board.hpp"
#include "../include/controller.hpp"
#include "../include/evaluate.hpp"
#include "../include/test.hpp"
#include "../include/search.hpp"
#include "../include/encodings.hpp"
#include "../include/stacktrace.hpp"

void seTranslator(unsigned int code, EXCEPTION_POINTERS* info) {
    std::cerr << "SEH exception caught: code " << code << std::endl;
    StackTrace::printStackTrace();
    exit(EXIT_FAILURE);
}

int main()
{
    // Set up the board using a FEN string.
    _set_se_translator(seTranslator);
    StackTrace::registerSignalHandler();
    AttackMask::initialize();  // Initialize attack masks
    ChessBoard board;
    Controller controller(board);
    controller.ParseFen(Fen::FEN_2);
    Evaluator evaluator(Evaluator::SymmetricEvaluation);
    SearchResult result = negamax(board, 4, evaluator, ChessEncoding::BLACK);
    Log::PrintMoveEncoding(result.best_move);
    std::cout << "Eval: " << result.evaluation << std::endl;
    // SearchResult result = negamax(board, 4, evaluator, ChessEncoding::WHITE);
    // Log::PrintMoveEncoding(result.best_move);
    // Log::PrintChessBoard(board);
    // std::vector<Move> moves = MoveGen::GetMoves(board, 1);
    // // Log::PrintMovesEncoding(moves);
    // // std::cout << moves.size() << std::endl;
    // Log::PrintMoveEncoding(moves[1]);
    // controller.MakeMove(moves[1], 1);
    // Log::PrintChessBoard(board);

    // std::cout << "P: " << MoveGen::GetPawnMoves(chess_board, 0).size() << std::endl;
    // std::cout << "p: " << MoveGen::GetPawnMoves(chess_board, 1).size() << std::endl;

    // std::cout << "K: " << MoveGen::GetKingMoves(chess_board, 0).size() << std::endl;
    // std::cout << "k: " << MoveGen::GetKingMoves(chess_board, 1).size() << std::endl;

    // Log::PrintMoves(chess_board, color);

    // Log::PrintMoves(chess_board, color);
    // Log::PrintAttackedSquares(chess_board, color);

    // std::vector<Move> moves = MoveGen::GetMoves(chess_board, color);
    // Log::printMovesEncoding(moves);
    // Log::PrintMoveEncoding(moves[4]);
    // controller.MakeMove(moves[4], color);
    // Log::PrintChessBoard(chess_board);
}'
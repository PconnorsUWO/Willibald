#include "../include/chess_board.hpp"
#include "../include/controller.hpp"
#include "../include/evaluate.hpp"
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>


static double NormalizeEvaluation(double score) {
    return 1.0 / (1.0 + std::exp(-score / 400.0));
}

class Test {
 public:
    // Runs evaluation tests.
    static void RunEvaluationTests() {
        std::cout << "Running Evaluation Tests..." << std::endl;
        ChessBoard board;
        Controller controller(board);
        std::string test_fen =
            "rn1qkbnr/ppp1pppp/8/3p4/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 0 3";
        controller.ParseFen(test_fen);
        Evaluator evaluator(Evaluator::SymmetricEvaluation);
        double score = evaluator.Evaluate(board);
        std::cout << "Evaluation score for test position: " << score << std::endl;
    }

    // Runs search function tests.
    static void RunSearchTests() {
        // std::cout << "Running Search Function Tests..." << std::endl;
        // ChessBoard board;
        // Controller controller(board);
        // std::string test_fen =
        //     "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        // controller.ParseFen(test_fen);
        std::cout << "Search test placeholder: search function called." << std::endl;
    }

    // Runs runtime performance tests.
    static void RunRuntimeTests() {
        std::cout << "Running Runtime Performance Tests..." << std::endl;
        // ChessBoard board;
        // Controller controller(board);
        // // Sample FEN for runtime testing.
        // std::string test_fen =
        //     "r1bqkbnr/pppp1ppp/2n5/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 2 3";
        // controller.ParseFen(test_fen);
        //
        // Evaluator evaluator(Evaluator::SymmetricEvaluation);
        // auto start_time = std::chrono::high_resolution_clock::now();
        // double score = evaluator.Evaluate(board);
        // auto end_time = std::chrono::high_resolution_clock::now();
        //
        // std::chrono::duration<double, std::milli> elapsed = end_time - start_time;
        // std::cout << "Evaluation score: " << score
        //           << " computed in " << elapsed.count() << " ms" << std::endl;
    }
};


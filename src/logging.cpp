//
// Created by Alexander King Perocho on 2025-03-18.
//
#include <iostream>
#include "../include/chess.h"
#include "../include/logging.h"
#include "../include/movegen.h"

void Log::PrintBitboard(const Bitboard bitboard) { bitboard.Print(); }

//TODO: Fix
void Log::PrintPieceOccupancy(const Chessboard& chessboard, int piece)
{
    for (int rank = 0; rank < 8; rank++)
    {
        std::cout << 8 - rank << "   ";
        for (int file = 0; file < 8; file++)
        {
            const int square = rank * 8 + file;
            std::cout << (chessboard.GetPieceOccupancy(piece).Test(square) ? Chess::ASCII_PIECES[piece] : '.')
                << ' ';
        }
        std::cout << ('\n');
    }
    std::cout << "\n    a b c d e f g h\n";
    std::cout << "\n decimal value = " << std::dec << chessboard.GetPieceOccupancy(piece) << '\n';
}

void Log::PrintChessboard(const Chessboard& chessboard)
{
    for (int rank = 0; rank < 8; rank++)
    {
        std::cout << 8 - rank << "   ";
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            char ascii = -1;
            for (int piece = Chess::P; piece <= Chess::k; piece++)
            {
                if (chessboard.GetPieceOccupancy(piece).Test(square))
                    ascii = Chess::ASCII_PIECES[piece];
            }
            std::cout << (ascii < 0 ? '.' : ascii) << ' ';
        }
        std::cout << ('\n');
    }
    std::cout << "\n    a b c d e f g h\n";
    std::cout << "\n    side:       " << (chessboard.GetSideToMove() ? "black" : "white");
    int eps = chessboard.GetEnpassantSquare();
    std::cout << "\n    enpassant:  " << (eps < 64 ? Chess::SQUARE_TO_ALGEBRAIC_NOTATION[eps] : "No Square");
    std::cout << "\n    castle:     "
        << (chessboard.GetCastlePrivilege() & Chess::wk ? "K" : "-")
        << (chessboard.GetCastlePrivilege() & Chess::wq ? "Q" : "-")
        << (chessboard.GetCastlePrivilege() & Chess::bk ? "k" : "-")
        << (chessboard.GetCastlePrivilege() & Chess::bq ? "q" : "-")
        << "\n\n";
}

void Log::PrintAttackedSquares(const Chessboard& chessboard, const int color)
{
    for (int rank = 0; rank < 8; rank++)
    {
        std::cout << 8 - rank << "   ";
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            std::cout << (MoveGen::IsAttacked(chessboard, square, color) ? 'x' : '.') << ' ';
        }
        std::cout << ('\n');
    }
    std::cout << "\n    a b c d e f g h\n";
    std::cout << "\n    attacks from: " << (color ? "black" : "white\n");
};

void Log::PrintMoves(const Chessboard& chessboard, const int color)
{
    const std::vector<Move> moves = MoveGen::GetMoves(chessboard, color);
    for (int rank = 0; rank < 8; rank++)
    {
        std::cout << 8 - rank << "   ";
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            int target_square = -1;
            for (const auto& move : moves)
            {
                target_square = move.Decode(Move::TargetSquare);
                if (square == target_square)
                    break;
                target_square = -1;
            }
            std::cout << (target_square >= 0 ? 'x' : '.') << ' ';
        }
        std::cout << ('\n');
    }
    std::cout << "\n    a b c d e f g h\n";
    std::cout << "\n    moves for: " << (color ? "black" : "white\n");
}

void Log::PrintPieceMoves(const Chessboard& chessboard, const int piece, const int color)
{
    const std::vector<Move> moves = MoveGen::GetMoves(chessboard, color);
    for (int rank = 0; rank < 8; rank++)
    {
        std::cout << 8 - rank << "   ";
        for (int file = 0; file < 8; file++)
        {
            const int square = rank * 8 + file;
            int target_square = -1;
            for (const auto& move : moves)
            {
                target_square = move.Decode(Move::TargetSquare);
                if (square == target_square)
                    break;
                target_square = -1;
            }
            std::cout << (target_square >= 0 ? 'x' : '.') << ' ';
        }
        std::cout << ('\n');
    }
    std::cout << "\n    a b c d e f g h\n";
    std::cout << "\n    Moves For: " << Chess::PIECE_NAME[piece] << '\n';
}

void Log::PrintMoveEncoding(const Move& move)
{
    int source_square = move.Decode(Move::SourceSquare);
    int target_square = move.Decode(Move::TargetSquare);
    int piece = move.Decode(Move::Piece);
    int promotion_piece = move.Decode(Move::PromotionPiece);
    int is_capture = move.Decode(Move::CaptureFlag);
    int is_double_pawn_push = move.Decode(Move::DoublePawnPushFlag);
    int is_castle = move.Decode(Move::CastleFlag);
    int is_enpassant = move.Decode(Move::EnpassantFlag);

    std::cout << Chess::ASCII_PIECES[piece]
        << ' '
        << Chess::SQUARE_TO_ALGEBRAIC_NOTATION[source_square]
        << ' '
        << Chess::SQUARE_TO_ALGEBRAIC_NOTATION[target_square]
        << "\nPromote: "
        << promotion_piece
        << "\nCapture: "
        << (is_capture ? "Yes" : "No")
        << "\nDPP: "
        << (is_double_pawn_push ? "Yes" : "No")
        << "\nCastle: "
        << (is_castle ? "Yes" : "No")
        << "\nEnpassant: "
        << (is_enpassant ? "Yes" : "No")
        << "\n\n";
};

void Log::PrintMovesEncoding(const std::vector<Move>& moves)
{
    for (int i = 0; i < moves.size(); i++)
    {
        const Move move = moves[i];
        std::cout << "\n\nMove: "
            << i
            << '\n';
        PrintMoveEncoding(move);
    }
};

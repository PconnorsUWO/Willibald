#include <iostream>
#include "../include/encodings.hpp"
#include "../include/logging.hpp"
#include "../include/movegen.hpp"

void Log::PrintBitboard(Bitboard bitboard) { bitboard.Print(); }

void Log::PrintPieceOccupancy(ChessBoard chess_board, int piece)
{
    for (int rank = 0; rank < 8; rank++)
    {
        std::cout << 8 - rank << "   ";
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            std::cout << (chess_board.GetPieceOccupancy(piece).Test(square) ? ChessEncoding::ASCII_PIECES[piece] : '.')
                      << ' ';
        }
        std::cout << ('\n');
    }
    std::cout << "\n    a b c d e f g h\n";
    std::cout << "\n decimal value = " << std::dec << chess_board.GetPieceOccupancy(piece) << '\n';
}

void Log::PrintChessBoard(ChessBoard chess_board)
{
    for (int rank = 0; rank < 8; rank++)
    {
        std::cout << 8 - rank << "   ";
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            char ascii = -1;
            for (int piece = ChessEncoding::P; piece <= ChessEncoding::k; piece++)
            {
                if (chess_board.GetPieceOccupancy(piece).Test(square))
                    ascii = ChessEncoding::ASCII_PIECES[piece];
            }
            std::cout << (ascii < 0 ? '.' : ascii) << ' ';
        }
        std::cout << ('\n');
    }
    std::cout << "\n    a b c d e f g h\n";
    std::cout << "\n    side:       " << (chess_board.GetSideToMove() ? "black" : "white");
    int eps = chess_board.GetEnpassantSquare();
    std::cout << "\n    enpassant:  " << (eps < 64 ? ChessEncoding::SQUARE_TO_ALGEBRAIC_NOTATION[eps] : "No Square");
    std::cout << "\n    castle:     "
              << (chess_board.GetCastlePrivelage() & ChessEncoding::wk ? "K" : "-")
              << (chess_board.GetCastlePrivelage() & ChessEncoding::wq ? "Q" : "-")
              << (chess_board.GetCastlePrivelage() & ChessEncoding::bk ? "k" : "-")
              << (chess_board.GetCastlePrivelage() & ChessEncoding::bq ? "q" : "-")
              << "\n\n";
}

void Log::PrintAttackedSquares(ChessBoard chess_board, int color)
{
    for (int rank = 0; rank < 8; rank++)
    {
        std::cout << 8 - rank << "   ";
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            std::cout << (MoveGen::IsAttacked(chess_board, square, color) ? 'x' : '.') << ' ';
        }
        std::cout << ('\n');
    }
    std::cout << "\n    a b c d e f g h\n";
    std::cout << "\n    attacks from: " << (color ? "black" : "white\n");
};

void Log::PrintMoves(ChessBoard chess_board, int color)
{
    std::vector<Move> moves = MoveGen::GetMoves(chess_board, color);
    for (int rank = 0; rank < 8; rank++)
    {
        std::cout << 8 - rank << "   ";
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            int target_square = -1;
            for (int i = 0; i < moves.size(); i++)
            {
                target_square = moves[i].Decode(Move::TargetSquare);
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

void Log::PrintMoveEncoding(Move move)
{
    int source_square = move.Decode(Move::SourceSquare);
    int target_square = move.Decode(Move::TargetSquare);
    int piece = move.Decode(Move::Piece);
    int promotion_piece = move.Decode(Move::PromotionPiece);
    int is_capture = move.Decode(Move::CaptureFlag);
    int is_double_pawn_push = move.Decode(Move::DoublePawnPushFlag);
    int is_castle = move.Decode(Move::CastleFlag);
    int is_enpassant = move.Decode(Move::EnpassantFlag);

    std::cout << ChessEncoding::ASCII_PIECES[piece]
              << ' '
              << ChessEncoding::SQUARE_TO_ALGEBRAIC_NOTATION[source_square]
              << ' '
              << ChessEncoding::SQUARE_TO_ALGEBRAIC_NOTATION[target_square]
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

void Log::PrintMovesEncoding(std::vector<Move> moves)
{
    for (int i = 0; i < moves.size(); i++)
    {
        Move move = moves[i];
        std::cout << "\n\nMove: "
                  << i
                  << '\n';
        PrintMoveEncoding(move);
    }
};
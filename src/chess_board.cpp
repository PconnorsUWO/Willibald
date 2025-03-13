#include <algorithm>
#include "../include/chess_board.hpp"
#include "../include/encodings.hpp"

ChessBoard::ChessBoard()
{
    enpassant_square = ChessEncoding::NO_SQUARE;
    castle_privelage = ChessEncoding::NO_CASTLE;
    side_to_move = 0;
}
ChessBoard ChessBoard::Copy() { return *this; };
Bitboard ChessBoard::GetPieceOccupancy(int piece) const { return piece_occupancies[piece]; }
Bitboard ChessBoard::GetColorOccupancy(int color) const { return color_occupancies[color]; }
int ChessBoard::GetSideToMove() const { return side_to_move; }
int ChessBoard::GetEnpassantSquare() const { return enpassant_square; }
int ChessBoard::GetCastlePrivelage() const { return castle_privelage; }
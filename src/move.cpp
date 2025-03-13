#include "../include/move.hpp"

Move::Move(int source_square, int target_square, int piece,
           int promotion_piece, int is_capture,
           int is_double_pawn_push, int is_enpassant, int is_castle)
{
    Encode(source_square, target_square, piece, promotion_piece, is_capture, is_double_pawn_push, is_enpassant, is_castle);
}
Move::operator int() const { return encoding; }
int Move::Encode(int source_square, int target_square, int piece,
             int promotion_piece, int is_capture,
             int is_double_pawn_push, int is_enpassant, int is_castle)
{
    encoding = (source_square |
                target_square << 6 |
                piece << 12 |
                promotion_piece << 16 |
                is_capture << 20 |
                is_double_pawn_push << 21 |
                is_enpassant << 22 |
                is_castle << 23);

    decodings[SourceSquare] = encoding & 0x3f;
    decodings[TargetSquare] = (encoding & 0xfc0) >> 6;
    decodings[Piece] = (encoding & 0xf000) >> 12;
    decodings[PromotionPiece] = (encoding & 0xf0000) >> 16;
    decodings[CaptureFlag] = encoding & 0x100000;
    decodings[DoublePawnPushFlag] = encoding & 0x200000;
    decodings[EnpassantFlag] = encoding & 0x400000;
    decodings[CastleFlag] = encoding & 0x800000;
    return encoding;
}
int Move::Decode(Encoding encoding) const { return decodings[encoding]; };
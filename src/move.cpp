//
// Created by Alexander King Perocho on 2025-03-18.
//
#include "../include/move.h"

Move::Move(const int source_square, const int target_square, const int piece,
           const int promotion_piece, const int is_capture,
           const int is_double_pawn_push, const int is_enpassant, const int is_castle)
{
    Encode(source_square, target_square, piece, promotion_piece, is_capture, is_double_pawn_push, is_enpassant,
           is_castle);
}

Move::operator int() const { return encoding; }

int Move::Encode(const int source_square, const int target_square, const int piece,
                 const int promotion_piece, const int is_capture,
                 const int is_double_pawn_push, const int is_enpassant, const int is_castle)
{
    encoding = (source_square |
        target_square << 6 |
        piece << 12 |
        promotion_piece << 16 |
        is_capture << 20 |
        is_double_pawn_push << 21 |
        is_enpassant << 22 |
        is_castle << 23);

    decoding[SourceSquare] = encoding & 0x3f;
    decoding[TargetSquare] = (encoding & 0xfc0) >> 6;
    decoding[Piece] = (encoding & 0xf000) >> 12;
    decoding[PromotionPiece] = (encoding & 0xf0000) >> 16;
    decoding[CaptureFlag] = encoding & 0x100000;
    decoding[DoublePawnPushFlag] = encoding & 0x200000;
    decoding[EnpassantFlag] = encoding & 0x400000;
    decoding[CastleFlag] = encoding & 0x800000;
    return encoding;
}

int Move::Decode(EncodingType encoding_type) const { return decoding[encoding_type]; }

#include "../include/moveutils.hpp"
int MoveUtils::encodeMove(int source_square, int target_square, int piece,
                          int promotion_piece, int is_capture,
                          int is_double_pawn_push, int is_enpassant, int is_castle)
{
    return (source_square |
            target_square << 6 |
            piece << 12 |
            promotion_piece << 16 |
            is_capture << 20 |
            is_double_pawn_push << 21 |
            is_enpassant << 22 |
            is_castle << 23);
};

int MoveUtils::decodeTargetSquare(int move) { return (move & 0xfc0) >> 6; }
int MoveUtils::decodeSourceSquare(int move) { return move & 0x3f; }
int MoveUtils::decodePiece(int move) { return (move & 0xf000) >> 12; }
int MoveUtils::decodePromotionPiece(int move) { return (move & 0xf0000) >> 16; }
int MoveUtils::decodeCaptureFlag(int move) { return move & 0x100000; }
int MoveUtils::decodeDoublePawnPushFlag(int move) { return move & 0x200000; }
int MoveUtils::decodeEnpassantFlag(int move) { return move & 0x400000; }
int MoveUtils::decodeCastleFlag(int move) { return move & 0x800000; }
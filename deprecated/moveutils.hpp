#pragma once
class MoveUtils
{
public:
    static int encodeMove(int source_square, int target_square, int piece,
                          int promotion_piece, int is_capture,
                          int is_double_pawn_push, int is_enpassant, int is_castle);
    static int decodeTargetSquare(int move);
    static int decodeSourceSquare(int move);
    static int decodePiece(int move);
    static int decodePromotionPiece(int move);
    static int decodeCaptureFlag(int move);
    static int decodeDoublePawnPushFlag(int move);
    static int decodeEnpassantFlag(int move);
    static int decodeCastleFlag(int move);
};
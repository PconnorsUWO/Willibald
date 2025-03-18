#pragma once

class Move
{
public:
    Move(int source_square, int target_square, int piece,
         int promotion_piece, int is_capture,
         int is_double_pawn_push, int is_enpassant, int is_castle);
    inline operator int() const;
    int Encode(int source_square, int target_square, int piece,
               int promotion_piece, int is_capture,
               int is_double_pawn_push, int is_enpassant, int is_castle);

    enum Encoding
    {
        SourceSquare,
        TargetSquare,
        Piece,
        PromotionPiece,
        CaptureFlag,
        DoublePawnPushFlag,
        EnpassantFlag,
        CastleFlag

    };
    int Decode(Encoding encoding) const;

private:
    int encoding;
    int decodings[8];
};
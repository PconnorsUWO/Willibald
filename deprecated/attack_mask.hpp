#pragma once

#include "bitboard.hpp"

class AttackMask
{
public:
    enum Mask
    {
        WhitePawn,
        BlackPawn,
        Knight,
        RawRook,
        RawBishop,
        Bishop,
        Rook,
        Queen,
        King,
    };

    static Bitboard GetAttackMask(Mask mask, int square);
    static Bitboard GetAttackMask(Mask mask, int square, Bitboard occupancy);
    static void initialize();

private:
    static inline const Bitboard NOT_A_FILE = 18374403900871474942ULL;
    static inline const Bitboard NOT_H_FILE = 9187201950435737471ULL;
    static inline const Bitboard NOT_HG_FILE = 4557430888798830399ULL;
    static inline const Bitboard NOT_AB_FILE = 18229723555195321596ULL;

    static Bitboard pawn_masks[2][64];
    static Bitboard knight_masks[64];
    static Bitboard bishop_masks[64][4096];
    static Bitboard rook_masks[64][4096];
    static Bitboard queen_masks[64][4096];
    static Bitboard king_masks[64];
    static Bitboard raw_bishop_masks[64];
    static Bitboard raw_rook_masks[64];
    static void InitializeMask(const Mask mask, const int square);
    static void InitializeSlideMask(const Mask mask, const int square);
    static Bitboard GetMaskedOccupancy(const int occupancy_index, const Bitboard attack_mask);
    AttackMask();
};
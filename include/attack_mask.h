//
// Created by Alexander King Perocho on 2025-03-10.
//

#ifndef ATTACK_MASK_H
#define ATTACK_MASK_H

#include "bitboard.h"

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
    static inline const Bitboard NOT_A_FILE = Bitboard(18374403900871474942ULL);
    static inline const Bitboard NOT_H_FILE = Bitboard(9187201950435737471ULL);
    static inline const Bitboard NOT_HG_FILE = Bitboard(4557430888798830399ULL);
    static inline const Bitboard NOT_AB_FILE = Bitboard(18229723555195321596ULL);

    static Bitboard pawn_masks[2][64];
    static Bitboard knight_masks[64];
    static Bitboard bishop_masks[64][4096];
    static Bitboard rook_masks[64][4096];
    static Bitboard queen_masks[64][4096];
    static Bitboard king_masks[64];
    static Bitboard raw_bishop_masks[64];
    static Bitboard raw_rook_masks[64];
    static void InitializeMask(Mask mask, int square);
    static void InitializeSlideMask(Mask mask, int square);
    static Bitboard GetMaskedOccupancy(int occupancy_index, Bitboard attack_mask);
    AttackMask();
};

#endif //ATTACK_MASK_H

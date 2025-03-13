// #pragma once

// #include "bitboard.hpp"

// class MaskGen
// {
//     static inline Bitboard NOT_A_FILE = 18374403900871474942ULL;
//     static inline Bitboard NOT_H_FILE = 9187201950435737471ULL;
//     static inline Bitboard NOT_HG_FILE = 4557430888798830399ULL;
//     static inline Bitboard NOT_AB_FILE = 18229723555195321596ULL;

// public:
//     static Bitboard evaluateMaskedOccupancy(int occupancy_index, Bitboard attack_mask);
//     static Bitboard evaluatePawnAttackMask(int color, int source_square);
//     static Bitboard evaluateKnightAttackMask(int source_square);
//     static Bitboard evaluateKingAttackMask(int source_square);
//     static Bitboard evaluateRawBishopAttackMask(int source_square);
//     static Bitboard evaluateBishopAttackMask(int source_square, Bitboard occupancy);
//     static Bitboard evaluateRawRookAttackMask(int source_square);
//     static Bitboard evaluateRookAttackMask(int source_square, Bitboard occupancy);
// };
// #pragma once
// #include "bitboard.hpp"
// #define BOARD_SIZE 64

// class AttackMaskUtils
// {
//     static Bitboard pawn_attack_masks[2][BOARD_SIZE];
//     static Bitboard knight_attack_masks[BOARD_SIZE];
//     static Bitboard king_attack_masks[BOARD_SIZE];
//     static Bitboard bishop_raw_attack_masks[BOARD_SIZE];
//     static Bitboard rook_raw_attack_masks[BOARD_SIZE];
//     static Bitboard bishop_attack_masks[BOARD_SIZE][BOARD_SIZE * BOARD_SIZE];
//     static Bitboard rook_attack_masks[BOARD_SIZE][BOARD_SIZE * BOARD_SIZE];
//     static Bitboard queen_attack_masks[BOARD_SIZE][BOARD_SIZE * BOARD_SIZE];
//     static void mapLeapAttackMasks();
//     static void mapSlideAttackMasks();
//     static Bitboard getRawBishopAttackMask(int source_square);
//     static Bitboard getRawRookAttackMask(int source_square);

// public:
//     static void mapAttackMasks();
//     static Bitboard getPawnAttackMask(int color, int source_square);
//     static Bitboard getKnightAttackMask(int source_square);
//     static Bitboard getKingAttackMask(int source_square);
//     static Bitboard getBishopAttackMask(int source_square, Bitboard occupany);
//     static Bitboard getRookAttackMask(int source_square, Bitboard occupany);
//     static Bitboard getQueenAttackMask(int source_square, Bitboard occupany);
// };
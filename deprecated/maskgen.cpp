// #include "../include/maskgen.hpp"
// Bitboard MaskGen::evaluateMaskedOccupancy(int occupancy_index, Bitboard attack_mask)
// {
//     Bitboard occupancy;
//     int num_attacks = attack_mask.CountBits();
//     for (int count = 0; count < num_attacks; count++)
//     {
//         int square = attack_mask.PopLsbIndex();
//         if (occupancy_index & (1 << count))
//         {
//             occupancy |= (1ULL << square);
//         }
//     }
//     return occupancy;
// }
// Bitboard MaskGen::evaluatePawnAttackMask(int color, int source_square)
// {
//     Bitboard bitboard;
//     Bitboard attack_mask;
//     bitboard.Set(source_square);
//     if (!color)
//     {
//         if (bitboard >> 7 & NOT_A_FILE)
//             attack_mask |= bitboard >> 7;
//         if (bitboard >> 9 & NOT_H_FILE)
//             attack_mask |= bitboard >> 9;
//     }
//     else
//     {
//         if (bitboard << 9 & NOT_A_FILE)
//             attack_mask |= bitboard << 9;
//         if (bitboard >> 7 & NOT_H_FILE)
//             attack_mask |= bitboard << 7;
//     }
//     return attack_mask;
// }
// Bitboard MaskGen::evaluateKnightAttackMask(int source_square)
// {
//     Bitboard bitboard;
//     Bitboard attack_mask;
//     bitboard.Set(source_square);
//     if (bitboard >> 6 & NOT_AB_FILE)
//         attack_mask |= bitboard >> 6;
//     if (bitboard << 6 & NOT_HG_FILE)
//         attack_mask |= bitboard << 6;
//     if (bitboard << 10 & NOT_AB_FILE)
//         attack_mask |= bitboard << 10;
//     if (bitboard >> 10 & NOT_HG_FILE)
//         attack_mask |= bitboard >> 10;
//     if (bitboard >> 15 & NOT_A_FILE)
//         attack_mask |= bitboard >> 15;
//     if (bitboard << 15 & NOT_H_FILE)
//         attack_mask |= bitboard << 15;
//     if (bitboard << 17 & NOT_A_FILE)
//         attack_mask |= bitboard << 17;
//     if (bitboard >> 17 & NOT_H_FILE)
//         attack_mask |= bitboard >> 17;
//     return attack_mask;
// }
// Bitboard MaskGen::evaluateKingAttackMask(int source_square)
// {
//     Bitboard bitboard;
//     Bitboard attack_mask;
//     bitboard.Set(source_square);
//     if (bitboard >> 8)
//         attack_mask |= bitboard >> 8;
//     if (bitboard << 8)
//         attack_mask |= bitboard << 8;
//     if (bitboard << 1 & NOT_A_FILE)
//         attack_mask |= bitboard << 1;
//     if (bitboard >> 1 & NOT_H_FILE)
//         attack_mask |= bitboard >> 1;
//     if (bitboard >> 7 & NOT_A_FILE)
//         attack_mask |= bitboard >> 7;
//     if (bitboard << 7 & NOT_H_FILE)
//         attack_mask |= bitboard << 7;
//     if (bitboard << 9 & NOT_A_FILE)
//         attack_mask |= bitboard << 9;
//     if (bitboard >> 9 & NOT_H_FILE)
//         attack_mask |= bitboard >> 9;
//     return attack_mask;
// }
// Bitboard MaskGen::evaluateRawBishopAttackMask(int source_square)
// {
//     Bitboard attack_mask;
//     int rank, file;
//     int target_rank = source_square / 8;
//     int target_file = source_square % 8;
//     for (rank = target_rank + 1, file = target_file + 1;
//          rank < 8 - 1 && file < 8 - 1;
//          rank++, file++)
//         attack_mask |= (1ULL << (rank * 8 + file));
//     for (rank = target_rank - 1, file = target_file + 1;
//          rank > 0 && file < 8 - 1;
//          rank--, file++)
//         attack_mask |= (1ULL << (rank * 8 + file));
//     for (rank = target_rank + 1, file = target_file - 1;
//          rank < 8 - 1 && file > 0;
//          rank++, file--)
//         attack_mask |= (1ULL << (rank * 8 + file));
//     for (rank = target_rank - 1, file = target_file - 1;
//          rank > 0 && file > 0;
//          rank--, file--)
//         attack_mask |= (1ULL << (rank * 8 + file));
//     return attack_mask;
// }
// Bitboard MaskGen::evaluateBishopAttackMask(int source_square, Bitboard occupancy)
// {
//     Bitboard attack_mask;
//     int rank, file;
//     int target_rank = source_square / 8;
//     int target_file = source_square % 8;

//     for (rank = target_rank + 1, file = target_file + 1;
//          rank < 8 && file < 8;
//          rank++, file++)
//     {
//         attack_mask |= (1ULL << (rank * 8 + file));
//         if ((1ULL << (rank * 8 + file)) & occupancy)
//             break;
//     }
//     for (rank = target_rank + 1, file = target_file - 1;
//          rank < 8 && file >= 0;
//          rank++, file--)
//     {
//         attack_mask |= (1ULL << (rank * 8 + file));
//         if ((1ULL << (rank * 8 + file)) & occupancy)
//             break;
//     }
//     for (rank = target_rank - 1, file = target_file + 1;
//          rank >= 0 && file < 8;
//          rank--, file++)
//     {
//         attack_mask |= (1ULL << (rank * 8 + file));
//         if ((1ULL << (rank * 8 + file)) & occupancy)
//             break;
//     }
//     for (rank = target_rank - 1, file = target_file - 1;
//          rank >= 0 && file >= 0;
//          rank--, file--)
//     {
//         attack_mask |= (1ULL << (rank * 8 + file));
//         if ((1ULL << (rank * 8 + file)) & occupancy)
//             break;
//     }
//     return attack_mask;
// }
// Bitboard MaskGen::evaluateRawRookAttackMask(int source_square)
// {
//     Bitboard attack_mask;
//     int rank, file;
//     int target_rank = source_square / 8;
//     int target_file = source_square % 8;
//     for (rank = target_rank + 1; rank < 8 - 1; rank++)
//         attack_mask |= (1ULL << (rank * 8 + target_file));
//     for (rank = target_rank - 1; rank > 0; rank--)
//         attack_mask |= (1ULL << (rank * 8 + target_file));
//     for (file = target_file + 1; file < 8 - 1; file++)
//         attack_mask |= (1ULL << (target_rank * 8 + file));
//     for (file = target_file - 1; file > 0; file--)
//         attack_mask |= (1ULL << (target_rank * 8 + file));
//     return attack_mask;
// }
// Bitboard MaskGen::evaluateRookAttackMask(int source_square, Bitboard occupancy)
// {
//     Bitboard attack_mask;
//     int rank, file;
//     int target_rank = source_square / 8;
//     int target_file = source_square % 8;
//     for (rank = target_rank + 1; rank < 8; rank++)
//     {
//         attack_mask |= (1ULL << (rank * 8 + target_file));
//         if ((1ULL << (rank * 8 + target_file)) & occupancy)
//             break;
//     }
//     for (rank = target_rank - 1; rank >= 0; rank--)
//     {
//         attack_mask |= (1ULL << (rank * 8 + target_file));
//         if ((1ULL << (rank * 8 + target_file)) & occupancy)
//             break;
//     }
//     for (file = target_file + 1; file < 8; file++)
//     {
//         attack_mask |= (1ULL << (target_rank * 8 + file));
//         if ((1ULL << (target_rank * 8 + file)) & occupancy)
//             break;
//     }
//     for (file = target_file - 1; file >= 0; file--)
//     {
//         attack_mask |= (1ULL << (target_rank * 8 + file));
//         if ((1ULL << (target_rank * 8 + file)) & occupancy)
//             break;
//     }
//     return attack_mask;
// }
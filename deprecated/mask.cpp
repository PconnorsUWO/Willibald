// #include <iostream>
// #include "../include/mask.hpp"
// #include "../include/encodings.hpp"
// #include "../include/maskgen.hpp"
// #include "../include/magic.hpp"

// Bitboard AttackMaskUtils::pawn_attack_masks[2][BOARD_SIZE];
// Bitboard AttackMaskUtils::knight_attack_masks[BOARD_SIZE];
// Bitboard AttackMaskUtils::king_attack_masks[BOARD_SIZE];
// Bitboard AttackMaskUtils::bishop_attack_masks[BOARD_SIZE][BOARD_SIZE * BOARD_SIZE];
// Bitboard AttackMaskUtils::rook_attack_masks[BOARD_SIZE][BOARD_SIZE * BOARD_SIZE];
// Bitboard AttackMaskUtils::queen_attack_masks[BOARD_SIZE][BOARD_SIZE * BOARD_SIZE];
// Bitboard AttackMaskUtils::bishop_raw_attack_masks[BOARD_SIZE];
// Bitboard AttackMaskUtils::rook_raw_attack_masks[BOARD_SIZE];
// void AttackMaskUtils::mapLeapAttackMasks()
// {
//     for (int source_square = ChessEncoding::a8; source_square <= ChessEncoding::h1; source_square++)
//     {
//         pawn_attack_masks[ChessEncoding::WHITE][source_square] =
//             MaskGen::evaluatePawnAttackMask(ChessEncoding::WHITE, source_square);
//         pawn_attack_masks[ChessEncoding::BLACK][source_square] =
//             MaskGen::evaluatePawnAttackMask(ChessEncoding::BLACK, source_square);
//         knight_attack_masks[source_square] =
//             MaskGen::evaluateKnightAttackMask(source_square);
//         king_attack_masks[source_square] =
//             MaskGen::evaluateKingAttackMask(source_square);
//     }
// }
// void AttackMaskUtils::mapSlideAttackMasks()
// {
//     for (int source_square = ChessEncoding::a8; source_square <= ChessEncoding::h1; source_square++)
//     {
//         bishop_raw_attack_masks[source_square] =
//             MaskGen::evaluateRawBishopAttackMask(source_square);
//         rook_raw_attack_masks[source_square] =
//             MaskGen::evaluateRawRookAttackMask(source_square);

//         Bitboard bishop_unblocked_mask = getRawBishopAttackMask(source_square);
//         for (int occupancy_index = 0;
//              occupancy_index < (1 << bishop_unblocked_mask.CountBits());
//              occupancy_index++)
//         {
//             Bitboard masked_occupancy =
//                 MaskGen::evaluateMaskedOccupancy(occupancy_index, bishop_unblocked_mask);
//             int magic_index =
//                 (masked_occupancy * Magic::BISHOP_MAGIC_NUMBERS[source_square]) >>
//                 (64 - Magic::BISHOP_ATTACK_COUNT_MASK[source_square]);

//             bishop_attack_masks[source_square][magic_index] =
//                 MaskGen::evaluateBishopAttackMask(source_square, masked_occupancy);
//             queen_attack_masks[source_square][magic_index] |=
//                 bishop_attack_masks[source_square][magic_index];
//         }


//         Bitboard rook_unblocked_attack_mask = rook_raw_attack_masks[source_square];
//         for (int occupancy_index = 0;
//              occupancy_index < (1 << rook_unblocked_attack_mask.CountBits());
//              occupancy_index++)
//         {
//             Bitboard masked_occupancy =
//                 MaskGen::evaluateMaskedOccupancy(occupancy_index, rook_unblocked_attack_mask);
//             int magic_index =
//                 (masked_occupancy * Magic::ROOK_MAGIC_NUMBERS[source_square]) >>
//                 (64 - Magic::ROOK_ATTACK_COUNT_MASK[source_square]);
//             rook_attack_masks[source_square][magic_index] =
//                 MaskGen::evaluateRookAttackMask(source_square, masked_occupancy);
//             queen_attack_masks[source_square][magic_index] |=
//                 rook_attack_masks[source_square][magic_index];
//         }
//     }
// }
// Bitboard AttackMaskUtils::getRawBishopAttackMask(int source_square) { return bishop_raw_attack_masks[source_square]; }
// Bitboard AttackMaskUtils::getRawRookAttackMask(int source_square) { return rook_raw_attack_masks[source_square]; }

// void AttackMaskUtils::mapAttackMasks()
// {
//     mapLeapAttackMasks();
//     mapSlideAttackMasks();
// }
// Bitboard AttackMaskUtils::getPawnAttackMask(int side, int source_square) { return pawn_attack_masks[side][source_square]; }
// Bitboard AttackMaskUtils::getKnightAttackMask(int source_square) { return knight_attack_masks[source_square]; }
// Bitboard AttackMaskUtils::getKingAttackMask(int source_square) { return king_attack_masks[source_square]; }
// Bitboard AttackMaskUtils::getBishopAttackMask(int source_square, Bitboard occupancy)
// {
//     occupancy &= bishop_raw_attack_masks[source_square];
//     occupancy *= Magic::BISHOP_MAGIC_NUMBERS[source_square];
//     occupancy >>= 64 - Magic::BISHOP_ATTACK_COUNT_MASK[source_square];
//     return bishop_attack_masks[source_square][occupancy];
// }
// Bitboard AttackMaskUtils::getRookAttackMask(int source_square, Bitboard occupancy)
// {
//     occupancy &= rook_raw_attack_masks[source_square];
//     occupancy *= Magic::ROOK_MAGIC_NUMBERS[source_square];
//     occupancy >>= 64 - Magic::ROOK_ATTACK_COUNT_MASK[source_square];
//     return rook_attack_masks[source_square][occupancy];
// };
// Bitboard AttackMaskUtils::getQueenAttackMask(int source_square, Bitboard occupancy) { return getBishopAttackMask(source_square, occupancy) | getRookAttackMask(source_square, occupancy); }

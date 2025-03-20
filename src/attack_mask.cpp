//
// Created by Alexander King Perocho on 2025-03-10.
//


#include "../include/attack_mask.h"
#include "../include/magic.h"

Bitboard AttackMask::king_masks[64];
Bitboard AttackMask::pawn_masks[2][64];
Bitboard AttackMask::knight_masks[64];
Bitboard AttackMask::raw_rook_masks[64];
Bitboard AttackMask::raw_bishop_masks[64];
Bitboard AttackMask::rook_masks[64][4096];
Bitboard AttackMask::queen_masks[64][4096];
Bitboard AttackMask::bishop_masks[64][4096];

Bitboard AttackMask::GetAttackMask(const Mask mask, const int square)
{
    switch (mask)
    {
    case WhitePawn:
        return pawn_masks[0][square];
    case BlackPawn:
        return pawn_masks[1][square];
    case Knight:
        return knight_masks[square];
    case RawBishop:
        return raw_bishop_masks[square];
    case RawRook:
        return raw_rook_masks[square];
    case King:
        return king_masks[square];
    default:
        return {};
    }
}

// Function 'GetAttackMask' is within a recursive call chain
Bitboard AttackMask::GetAttackMask(const Mask mask, const int square, Bitboard occupancy)
{
    switch (mask)
    {
    case Bishop:
        {
            occupancy &= raw_bishop_masks[square];
            occupancy *= Magic::BISHOP_MAGIC_NUMBERS[square];
            occupancy >>= 64 - Magic::BISHOP_ATTACK_COUNT_MASK[square];
            return bishop_masks[square][occupancy];
        }
    case Rook:
        {
            // get rook attacks assuming current board occupancy
            occupancy &= raw_rook_masks[square];
            occupancy *= Magic::ROOK_MAGIC_NUMBERS[square];
            occupancy >>= 64 - Magic::ROOK_ATTACK_COUNT_MASK[square];
            return rook_masks[square][occupancy];
        }
    case Queen:
        {
            const Bitboard bishop_attacks = GetAttackMask(Bishop, square, occupancy);
            const Bitboard rook_attacks = GetAttackMask(Rook, square, occupancy);
            return bishop_attacks | rook_attacks;
        }
    default:
        return {};
    }
}

Bitboard AttackMask::GetMaskedOccupancy(const int occupancy_index, Bitboard attack_mask, const int num_attacks)
{
    Bitboard occupancy;
    for (int count = 0; count < num_attacks; count++)
    {
        const int square = attack_mask.PopLsbIndex();
        if (occupancy_index & (1 << count))
            occupancy |= (1ULL << square);
    }
    return occupancy;
}

//TODO: FIX BISHOP AND ROOK ATTACK MASK INDEXING/GENERATION
void AttackMask::InitializeMask(const Mask mask, const int square)
{
    Bitboard bitboard;
    Bitboard attack_mask;
    bitboard.Set(square);

    int rank, file;
    const int target_rank = square / 8;
    const int target_file = square % 8;

    switch (mask)
    {
    case WhitePawn:
        {
            pawn_masks[0][square] |=
                (NOT_A_FILE & (1ULL << (square - 7))) |
                (NOT_H_FILE & (1ULL << (square - 9)));
            return;
        }
    case BlackPawn:
        {
            // if (bitboard << 9 & NOT_A_FILE)
            //     attack_mask |= bitboard << 9;
            // if (bitboard >> 7 & NOT_H_FILE)
            //     attack_mask |= bitboard << 7;
            // pawn_masks[1][square] = attack_mask;
            pawn_masks[1][square] |=
                (NOT_A_FILE & (1ULL << (square + 9))) |
                (NOT_H_FILE & (1ULL << (square + 7)));
            return;
        }
    case Knight:
        {
            knight_masks[square] |=
                (bitboard >> 6 & NOT_AB_FILE) |
                (bitboard << 6 & NOT_HG_FILE) |
                (bitboard << 10 & NOT_AB_FILE) |
                (bitboard >> 10 & NOT_HG_FILE) |
                (bitboard >> 15 & NOT_A_FILE) |
                (bitboard << 15 & NOT_H_FILE) |
                (bitboard << 17 & NOT_A_FILE) |
                (bitboard >> 17 & NOT_H_FILE);
            // if (bitboard >> 6 & NOT_AB_FILE)
            //     attack_mask |= bitboard >> 6;
            // if (bitboard << 6 & NOT_HG_FILE)
            //     attack_mask |= bitboard << 6;
            // if (bitboard << 10 & NOT_AB_FILE)
            //     attack_mask |= bitboard << 10;
            // if (bitboard >> 10 & NOT_HG_FILE)
            //     attack_mask |= bitboard >> 10;
            // if (bitboard >> 15 & NOT_A_FILE)
            //     attack_mask |= bitboard >> 15;
            // if (bitboard << 15 & NOT_H_FILE)
            //     attack_mask |= bitboard << 15;
            // if (bitboard << 17 & NOT_A_FILE)
            //     attack_mask |= bitboard << 17;
            // if (bitboard >> 17 & NOT_H_FILE)
            //     attack_mask |= bitboard >> 17;
            // knight_masks[square] = attack_mask;
            return;
        }
    case RawBishop:
        {
            for (rank = target_rank + 1, file = target_file + 1;
                 rank < 8 - 1 && file < 8 - 1;
                 rank++, file++)
                attack_mask |= (1ULL << (rank * 8 + file));
            for (rank = target_rank - 1, file = target_file + 1;
                 rank > 0 && file < 8 - 1;
                 rank--, file++)
                attack_mask |= (1ULL << (rank * 8 + file));
            for (rank = target_rank + 1, file = target_file - 1;
                 rank < 8 - 1 && file > 0;
                 rank++, file--)
                attack_mask |= (1ULL << (rank * 8 + file));
            for (rank = target_rank - 1, file = target_file - 1;
                 rank > 0 && file > 0;
                 rank--, file--)
                attack_mask |= (1ULL << (rank * 8 + file));
            raw_bishop_masks[square] = attack_mask;
            return;
        }
    case RawRook:
        {
            for (rank = target_rank + 1; rank < 8 - 1; rank++)
                attack_mask |= (1ULL << (rank * 8 + target_file));
            for (rank = target_rank - 1; rank > 0; rank--)
                attack_mask |= (1ULL << (rank * 8 + target_file));
            for (file = target_file + 1; file < 8 - 1; file++)
                attack_mask |= (1ULL << (target_rank * 8 + file));
            for (file = target_file - 1; file > 0; file--)
                attack_mask |= (1ULL << (target_rank * 8 + file));
            raw_rook_masks[square] = attack_mask;
            return;
        }
    case Bishop:
        {
            const int num_attacks = raw_bishop_masks[square].CountBits();
            const Bitboard raw_bishop_mask = raw_bishop_masks[square];
            const Bitboard magic_bitboard = Magic::BISHOP_MAGIC_NUMBERS[square];
            const int magic_shift = 64 - Magic::BISHOP_ATTACK_COUNT_MASK[square];

            for (int occupancy_index = 0; occupancy_index < (1 << num_attacks); occupancy_index++)
            {
                attack_mask = 0ULL;
                Bitboard masked_occupancy = GetMaskedOccupancy(occupancy_index, raw_bishop_mask, num_attacks);
                const int magic_index = (masked_occupancy * magic_bitboard[square]) >> magic_shift;

                for (rank = target_rank + 1, file = target_file + 1;
                     rank < 8 && file < 8;
                     rank++, file++)
                {
                    attack_mask |= (1ULL << (rank * 8 + file));
                    if ((1ULL << (rank * 8 + file)) & masked_occupancy)
                        break;
                }
                for (rank = target_rank + 1, file = target_file - 1;
                     rank < 8 && file >= 0;
                     rank++, file--)
                {
                    attack_mask |= (1ULL << (rank * 8 + file));
                    if ((1ULL << (rank * 8 + file)) & masked_occupancy)
                        break;
                }
                for (rank = target_rank - 1, file = target_file + 1;
                     rank >= 0 && file < 8;
                     rank--, file++)
                {
                    attack_mask |= (1ULL << (rank * 8 + file));
                    if ((1ULL << (rank * 8 + file)) & masked_occupancy)
                        break;
                }
                for (rank = target_rank - 1, file = target_file - 1;
                     rank >= 0 && file >= 0;
                     rank--, file--)
                {
                    attack_mask |= (1ULL << (rank * 8 + file));
                    if ((1ULL << (rank * 8 + file)) & masked_occupancy)
                        break;
                }
                bishop_masks[square][magic_index] = attack_mask;
                queen_masks[square][magic_index] |= attack_mask;
            }
            return;
        }
    case Rook:
        {
            const int num_attacks = raw_rook_masks[square].CountBits();
            const Bitboard raw_rook_mask = raw_rook_masks[square];
            const int magic_bitboard = Magic::ROOK_MAGIC_NUMBERS[square];
            const int magic_shift = 64 - Magic::ROOK_ATTACK_COUNT_MASK[square];
            for (int occupancy_index = 0; occupancy_index < (1 << num_attacks); occupancy_index++)
            {
                attack_mask = 0ULL;
                Bitboard masked_occupancy = GetMaskedOccupancy(occupancy_index, raw_rook_mask, num_attacks);
                const int magic_index = (masked_occupancy * magic_bitboard) >> (magic_shift);
                for (rank = target_rank + 1; rank < 8; rank++)
                {
                    attack_mask |= (1ULL << (rank * 8 + target_file));
                    if ((1ULL << (rank * 8 + target_file)) & masked_occupancy)
                        break;
                }
                for (rank = target_rank - 1; rank >= 0; rank--)
                {
                    attack_mask |= (1ULL << (rank * 8 + target_file));
                    if ((1ULL << (rank * 8 + target_file)) & masked_occupancy)
                        break;
                }
                for (file = target_file + 1; file < 8; file++)
                {
                    attack_mask |= (1ULL << (target_rank * 8 + file));
                    if ((1ULL << (target_rank * 8 + file)) & masked_occupancy)
                        break;
                }
                for (file = target_file - 1; file >= 0; file--)
                {
                    attack_mask |= (1ULL << (target_rank * 8 + file));
                    if ((1ULL << (target_rank * 8 + file)) & masked_occupancy)
                        break;
                }
                rook_masks[square][magic_index] = attack_mask;
                queen_masks[square][magic_index] |= attack_mask;
            }
            return;
        }
    case King:
        {
            king_masks[square] |=
                (bitboard >> 8) |
                (bitboard << 8) |
                (bitboard << 1 & NOT_A_FILE) |
                (bitboard >> 1 & NOT_H_FILE) |
                (bitboard >> 7 & NOT_A_FILE) |
                (bitboard << 7 & NOT_H_FILE) |
                (bitboard << 9 & NOT_A_FILE) |
                (bitboard >> 9 & NOT_H_FILE);
            // if (bitboard >> 8)
            //     attack_mask |= bitboard >> 8;
            // if (bitboard << 8)
            //     attack_mask |= bitboard << 8;
            // if (bitboard << 1 & NOT_A_FILE)
            //     attack_mask |= bitboard << 1;
            // if (bitboard >> 1 & NOT_H_FILE)
            //     attack_mask |= bitboard >> 1;
            // if (bitboard >> 7 & NOT_A_FILE)
            //     attack_mask |= bitboard >> 7;
            // if (bitboard << 7 & NOT_H_FILE)
            //     attack_mask |= bitboard << 7;
            // if (bitboard << 9 & NOT_A_FILE)
            //     attack_mask |= bitboard << 9;
            // if (bitboard >> 9 & NOT_H_FILE)
            //     attack_mask |= bitboard >> 9;
            // king_masks[square] = attack_mask;
            return;
        }
    default:
        return;
    }
}

void AttackMask::initialize()
{
    for (int mask = WhitePawn; mask <= King; mask++)
        for (int square = 0; square < 64; square++)
            InitializeMask(static_cast<Mask>(mask), square);
}

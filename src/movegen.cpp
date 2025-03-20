//
// Created by Alexander King Perocho on 2025-03-18.
//
#include "../include/movegen.h"
#include "../include/attack_mask.h"
#include "../include/chess.h"
#include "../include/chessboard.h"
#include "../include/controller.h"
#include "../include/logging.h"
#include <iostream>
bool MoveGen::IsAttacked(const Chessboard& chessboard, const int square, const int color)
{
    const Bitboard pawn_attack_mask = AttackMask::GetAttackMask(color ? AttackMask::BlackPawn : AttackMask::WhitePawn, square);
    const Bitboard knight_attack_mask = AttackMask::GetAttackMask(AttackMask::Knight, square);
    const Bitboard king_attack_mask = AttackMask::GetAttackMask(AttackMask::King, square);
    const Bitboard bishop_attack_mask = AttackMask::GetAttackMask(AttackMask::Bishop, square,
                                                                  chessboard.GetColorOccupancy(Chess::BOTH));
    const Bitboard rook_attack_mask = AttackMask::GetAttackMask(AttackMask::Rook, square,
                                                                chessboard.GetColorOccupancy(Chess::BOTH));
    const Bitboard queen_attack_mask = AttackMask::GetAttackMask(AttackMask::Queen, square,
                                                                 chessboard.GetColorOccupancy(Chess::BOTH));

    if (pawn_attack_mask &
        chessboard.GetPieceOccupancy(color ? Chess::p : Chess::P))
        return true;
    if (knight_attack_mask &
        chessboard.GetPieceOccupancy(color ? Chess::n : Chess::N))
        return true;
    if (king_attack_mask &
        chessboard.GetPieceOccupancy(color ? Chess::k : Chess::K))
        return true;
    if (bishop_attack_mask &
        chessboard.GetPieceOccupancy(color ? Chess::b : Chess::B))
        return true;
    if (rook_attack_mask &
        chessboard.GetPieceOccupancy(color ? Chess::r : Chess::R))
        return true;
    if (queen_attack_mask &
        chessboard.GetPieceOccupancy(color ? Chess::q : Chess::Q))
        return true;
    return false;
}

bool MoveGen::IsInCheck(const Chessboard& chessboard, const int color)
{
    const int king_piece = color ? Chess::k : Chess::K;
    int king_square = chessboard.GetPieceOccupancy(king_piece).GetLsbIndex();
    return IsAttacked(chessboard, king_square, !color);
}

std::vector<Move> MoveGen::GetLegalMoves(const Chessboard& chessboard, const int color)
{
    std::vector<Move> pseudo_legal_moves = GetMoves(chessboard, color);
    std::vector<Move> legal_moves;
    for (const Move& move : pseudo_legal_moves)
    {
        Chessboard temp_board = chessboard.Copy();
        Controller temp_controller(temp_board);
        temp_controller.MakeMove(move, color);
        if (!IsInCheck(temp_board, color))
        {
            legal_moves.push_back(move);
        }
    }
    return legal_moves;
}


std::vector<Move> MoveGen::GetPawnMoves(const Chessboard& chessboard, const int color)
{
    std::vector<Move> moves;
    const int piece = color ? Chess::p : Chess::P;
    Bitboard pawn_occupancy = chessboard.GetPieceOccupancy(piece);
    const Bitboard current_occupancy = chessboard.GetColorOccupancy(Chess::BOTH);
    int direction = color ? 1 : -1;
    while (pawn_occupancy)
    {
        int source_square = pawn_occupancy.PopLsbIndex();
        int target_square = source_square + 8 * direction;
        int double_push_target_square = target_square + 8 * direction;
        Bitboard pawn_attack_mask = AttackMask::GetAttackMask(color ? AttackMask::BlackPawn : AttackMask::WhitePawn,
                                                              source_square) & chessboard.GetColorOccupancy(!color);
        if (!current_occupancy.Test(target_square))
        {
            if ((color && source_square >= Chess::a7 && source_square <= Chess::h7 ||
                    !color && source_square >= Chess::a2 && source_square <= Chess::h2) &&
                !(current_occupancy.Test(double_push_target_square)))
                moves.emplace_back(source_square, double_push_target_square, piece, 0, 0, 1, 0, 0);
            if ((color && (source_square >= Chess::a2 && source_square <= Chess::h2)) ||
                (!color && (source_square >= Chess::a8 && source_square <= Chess::h8)))
            {
                moves.emplace_back(source_square, target_square, piece, Chess::Q, 0, 0, 0, 0);
                moves.emplace_back(source_square, target_square, piece, Chess::R, 0, 0, 0, 0);
                moves.emplace_back(source_square, target_square, piece, Chess::B, 0, 0, 0, 0);
                moves.emplace_back(source_square, target_square, piece, Chess::N, 0, 0, 0, 0);
            }
            moves.emplace_back(source_square, target_square, piece, 0, 0, 0, 0, 0);
        }
        while (pawn_attack_mask)
        {
            target_square = pawn_attack_mask.PopLsbIndex();
            if ((color && (source_square >= Chess::a2 && source_square <= Chess::h2)) ||
                (!color && (source_square >= Chess::a8 && source_square <= Chess::h8)))
            {
                moves.emplace_back(source_square, target_square, piece, Chess::Q, 1, 0, 0, 0);
                moves.emplace_back(source_square, target_square, piece, Chess::R, 1, 0, 0, 0);
                moves.emplace_back(source_square, target_square, piece, Chess::B, 1, 0, 0, 0);
                moves.emplace_back(source_square, target_square, piece, Chess::N, 1, 0, 0, 0);
            }
            else
                moves.emplace_back(source_square, target_square, piece, 0, 1, 0, 0, 0);
        }
        if (!chessboard.GetEnpassantSquare())
            continue;
        Bitboard enpassant_attack_mask =
            AttackMask::GetAttackMask(color ? AttackMask::BlackPawn : AttackMask::WhitePawn, source_square) & (1ULL <<
                chessboard.GetEnpassantSquare());
        if (enpassant_attack_mask)
        {
            target_square = enpassant_attack_mask.GetLsbIndex();
            moves.emplace_back(source_square, target_square, piece, 0, 1, 0, 1, 0);
        }
    }
    return moves;
}


//TODO: Check Castle Implementation.
std::vector<Move> MoveGen::GetKingMoves(Chessboard chessboard, int color)
{
    std::vector<Move> moves;
    const int piece = color ? Chess::k : Chess::K;
    const int source_square = chessboard.GetPieceOccupancy(piece).PopLsbIndex();
    const Bitboard current_occupancy = chessboard.GetColorOccupancy(Chess::BOTH);
    const int castle_privilege = chessboard.GetCastlePrivilege();

    if (!IsAttacked(chessboard, source_square, !color) &&
        (castle_privilege & Chess::wk || castle_privilege & Chess::bk))
    {
        if (!current_occupancy.Test(source_square + 1) &&
            !current_occupancy.Test(source_square + 2) &&
            !IsAttacked(chessboard, source_square + 1, !color) &&
            !IsAttacked(chessboard, source_square + 2, !color))
            moves.emplace_back(source_square, source_square + 2, piece, 0, 0, 0, 0, 1);
        if (!current_occupancy.Test(source_square - 1) &&
            !current_occupancy.Test(source_square - 2) &&
            !current_occupancy.Test(source_square - 3) &&
            !IsAttacked(chessboard, source_square - 1, !color) &&
            !IsAttacked(chessboard, source_square - 2, !color))
            moves.emplace_back(source_square, source_square - 2, piece, 0, 0, 0, 0, 1);
    }

    Bitboard king_attack_mask = AttackMask::GetAttackMask(AttackMask::King, source_square);
    while (king_attack_mask)
    {
        int target_square = king_attack_mask.PopLsbIndex();
        Bitboard color_occupancy = chessboard.GetColorOccupancy(color);
        if (!IsAttacked(chessboard, target_square, !color) && !color_occupancy.Test(target_square))
            moves.emplace_back(source_square, target_square, piece, 0, 0, 0, 0, 0);
    }
    return moves;
}

std::vector<Move> MoveGen::GetKnightMoves(const Chessboard& chessboard, const int color)
{
    std::vector<Move> moves;
    int piece = color ? Chess::n : Chess::N;
    Bitboard knight_occupancy = chessboard.GetPieceOccupancy(piece);
    while (knight_occupancy)
    {
        int source_square = knight_occupancy.PopLsbIndex();
        Bitboard knight_attack_mask = AttackMask::GetAttackMask(AttackMask::Knight, source_square);
        while (knight_attack_mask)
        {
            int target_square = knight_attack_mask.PopLsbIndex();
            if (!chessboard.GetColorOccupancy(Chess::BOTH).Test(target_square))
                moves.emplace_back(source_square, target_square, piece, 0, 0, 0, 0, 0);
            if (chessboard.GetColorOccupancy(!color).Test(target_square))
                moves.emplace_back(source_square, target_square, piece, 0, 1, 0, 0, 0);
        }
    }
    return moves;
}

std::vector<Move> MoveGen::GetBishopMoves(const Chessboard& chessboard, const int color)
{
    std::vector<Move> moves;
    int piece = color ? Chess::b : Chess::B;
    Bitboard bishop_occupancy = chessboard.GetPieceOccupancy(piece);
    Bitboard current_occupancy = chessboard.GetColorOccupancy(Chess::BOTH);
    while (bishop_occupancy)
    {
        int source_square = bishop_occupancy.PopLsbIndex();
        Bitboard bishop_attack_mask = AttackMask::GetAttackMask(AttackMask::Bishop, source_square, current_occupancy);
        while (bishop_attack_mask)
        {
            int target_square = bishop_attack_mask.PopLsbIndex();
            if (!current_occupancy.Test(target_square))
                moves.emplace_back(source_square, target_square, piece, 0, 0, 0, 0, 0);
            if (chessboard.GetColorOccupancy(!color).Test(target_square))
                moves.emplace_back(source_square, target_square, piece, 0, 1, 0, 0, 0);
        }
    }
    return moves;
}

std::vector<Move> MoveGen::GetRookMoves(const Chessboard& chessboard, int color)
{
    std::vector<Move> moves;
    int piece = color ? Chess::r : Chess::R;
    Bitboard rook_occupancy = chessboard.GetPieceOccupancy(piece);
    Bitboard current_occupancy = chessboard.GetColorOccupancy(Chess::BOTH);
    while (rook_occupancy)
    {
        int source_square = rook_occupancy.PopLsbIndex();
        Bitboard rook_attack_mask = AttackMask::GetAttackMask(AttackMask::Rook, source_square, current_occupancy);
        while (rook_attack_mask)
        {
            int target_square = rook_attack_mask.PopLsbIndex();
            if (!current_occupancy.Test(target_square))
                moves.emplace_back(source_square, target_square, piece, 0, 0, 0, 0, 0);
            if (chessboard.GetColorOccupancy(!color).Test(target_square))
                moves.emplace_back(source_square, target_square, piece, 0, 1, 0, 0, 0);
        }
    }
    return moves;
}

std::vector<Move> MoveGen::GetQueenMoves(const Chessboard& chessboard, const int color)
{
    std::vector<Move> moves;
    int piece = color ? Chess::q : Chess::Q;
    Bitboard queen_occupancy = chessboard.GetPieceOccupancy(piece);
    Bitboard current_occupancy = chessboard.GetColorOccupancy(Chess::BOTH);
    while (queen_occupancy)
    {
        int source_square = queen_occupancy.PopLsbIndex();
        Bitboard queen_attack_mask = AttackMask::GetAttackMask(AttackMask::Queen, source_square, current_occupancy);
        while (queen_attack_mask)
        {
            int target_square = queen_attack_mask.PopLsbIndex();;
            if (!current_occupancy.Test(target_square))
                moves.emplace_back(source_square, target_square, piece, 0, 0, 0, 0, 0);
            if (chessboard.GetColorOccupancy(!color).Test(target_square))
                moves.emplace_back(source_square, target_square, piece, 0, 1, 0, 0, 0);
        }
    }
    return moves;
}

std::vector<Move> MoveGen::GetMoves(const Chessboard& chessboard, const int color)
{
    std::vector<Move> moves = GetPawnMoves(chessboard, color);
    std::vector<Move> king_moves = GetKingMoves(chessboard, color);
    std::vector<Move> knight_moves = GetKnightMoves(chessboard, color);
    std::vector<Move> bishop_moves = GetBishopMoves(chessboard, color);
    std::vector<Move> rook_moves = GetRookMoves(chessboard, color);
    std::vector<Move> queen_moves = GetQueenMoves(chessboard, color);
    moves.insert(moves.end(), king_moves.begin(), king_moves.end());
    moves.insert(moves.end(), knight_moves.begin(), knight_moves.end());
    moves.insert(moves.end(), bishop_moves.begin(), bishop_moves.end());
    moves.insert(moves.end(), rook_moves.begin(), rook_moves.end());
    moves.insert(moves.end(), queen_moves.begin(), queen_moves.end());
    return moves;
}

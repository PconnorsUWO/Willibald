#include "../include/movegen.hpp"
#include "../include/attack_mask.hpp"
#include "../include/encodings.hpp"

bool MoveGen::IsAttacked(ChessBoard chess_board, int square, int color)
{
    Bitboard pawn_attack_mask = AttackMask::GetAttackMask(color ? AttackMask::BlackPawn : AttackMask::WhitePawn, square);
    Bitboard knight_attack_mask = AttackMask::GetAttackMask(AttackMask::Knight, square);
    Bitboard king_attack_mask = AttackMask::GetAttackMask(AttackMask::King, square);

    // Segmentation Fault
    Bitboard bishop_attack_mask = AttackMask::GetAttackMask(AttackMask::Bishop, square, chess_board.GetColorOccupancy(ChessEncoding::BOTH));
    Bitboard rook_attack_mask = AttackMask::GetAttackMask(AttackMask::Rook, square, chess_board.GetColorOccupancy(ChessEncoding::BOTH));
    Bitboard queen_attack_mask = AttackMask::GetAttackMask(AttackMask::Queen, square, chess_board.GetColorOccupancy(ChessEncoding::BOTH));

    if (pawn_attack_mask &
        chess_board.GetPieceOccupancy(color ? ChessEncoding::p : ChessEncoding::P))
        return true;
    if (knight_attack_mask &
        chess_board.GetPieceOccupancy(color ? ChessEncoding::n : ChessEncoding::N))
        return true;
    if (king_attack_mask &
        chess_board.GetPieceOccupancy(color ? ChessEncoding::k : ChessEncoding::K))
        return true;
    if (bishop_attack_mask &
        chess_board.GetPieceOccupancy(color ? ChessEncoding::b : ChessEncoding::B))
        return true;
    if (rook_attack_mask &
        chess_board.GetPieceOccupancy(color ? ChessEncoding::r : ChessEncoding::R))
        return true;
    if (queen_attack_mask &
        chess_board.GetPieceOccupancy(color ? ChessEncoding::q : ChessEncoding::Q))
        return true;
    return false;
}

std::vector<Move> MoveGen::GetPawnMoves(ChessBoard chess_board, int color)
{
    std::vector<Move> moves;
    int piece = color ? ChessEncoding::p : ChessEncoding::P;
    Bitboard pawn_occupancy = chess_board.GetPieceOccupancy(piece);
    Bitboard current_occupancy = chess_board.GetColorOccupancy(ChessEncoding::BOTH);
    int direction = color ? 1 : -1;
    while (pawn_occupancy)
    {
        int source_square = pawn_occupancy.PopLsbIndex();
        int target_square = source_square + 8 * direction;
        int double_push_target_square = target_square + 8 * direction;
        Bitboard pawn_attack_mask = AttackMask::GetAttackMask(color ? AttackMask::BlackPawn : AttackMask::WhitePawn, source_square) & chess_board.GetColorOccupancy(!color);
        if (!current_occupancy.Test(target_square))
        {
            if ((color && source_square >= ChessEncoding::a7 && source_square <= ChessEncoding::h7 ||
                 !color && source_square >= ChessEncoding::a2 && source_square <= ChessEncoding::h2) &&
                !(current_occupancy.Test(double_push_target_square)))
                moves.push_back(Move(source_square, double_push_target_square, piece, 0, 0, 1, 0, 0));
            if ((color && (source_square >= ChessEncoding::a2 && source_square <= ChessEncoding::h2)) ||
                (!color && (source_square >= ChessEncoding::a8 && source_square <= ChessEncoding::h8)))
            {
                moves.push_back(Move(source_square, target_square, piece, ChessEncoding::Q, 0, 0, 0, 0));
                moves.push_back(Move(source_square, target_square, piece, ChessEncoding::R, 0, 0, 0, 0));
                moves.push_back(Move(source_square, target_square, piece, ChessEncoding::B, 0, 0, 0, 0));
                moves.push_back(Move(source_square, target_square, piece, ChessEncoding::N, 0, 0, 0, 0));
            }
            moves.push_back(Move(source_square, target_square, piece, 0, 0, 0, 0, 0));
        }
        while (pawn_attack_mask)
        {
            target_square = pawn_attack_mask.PopLsbIndex();
            if ((color && (source_square >= ChessEncoding::a2 && source_square <= ChessEncoding::h2)) ||
                (!color && (source_square >= ChessEncoding::a8 && source_square <= ChessEncoding::h8)))
            {
                moves.push_back(Move(source_square, target_square, piece, ChessEncoding::Q, 1, 0, 0, 0));
                moves.push_back(Move(source_square, target_square, piece, ChessEncoding::R, 1, 0, 0, 0));
                moves.push_back(Move(source_square, target_square, piece, ChessEncoding::B, 1, 0, 0, 0));
                moves.push_back(Move(source_square, target_square, piece, ChessEncoding::N, 1, 0, 0, 0));
            }
            else
                moves.push_back(Move(source_square, target_square, piece, 0, 1, 0, 0, 0));
        }
        if (!chess_board.GetEnpassantSquare())
            continue;
        Bitboard enpassant_attack_mask =
            AttackMask::GetAttackMask(color ? AttackMask::BlackPawn : AttackMask::WhitePawn, source_square) & (1ULL << chess_board.GetEnpassantSquare());
        if (enpassant_attack_mask)
        {
            target_square = enpassant_attack_mask.GetLsbIndex();
            moves.push_back(Move(source_square, target_square, piece, 0, 1, 0, 1, 0));
        }
    }
    return moves;
}

std::vector<Move> MoveGen::GetKingMoves(ChessBoard chess_board, int color)
{
    std::vector<Move> moves;
    const int piece = color ? ChessEncoding::k : ChessEncoding::K;
    const int source_square = chess_board.GetPieceOccupancy(piece).PopLsbIndex();
    const Bitboard current_occupancy = chess_board.GetColorOccupancy(ChessEncoding::BOTH);
    const int castle_privelage = chess_board.GetCastlePrivelage();

    if (!IsAttacked(chess_board, source_square, color) &&
        (castle_privelage & ChessEncoding::wk || castle_privelage & ChessEncoding::bk))
    {
        if (!current_occupancy.Test(source_square + 1) &&
            !current_occupancy.Test(source_square + 2) &&
            !IsAttacked(chess_board, source_square + 1, !color) &&
            !IsAttacked(chess_board, source_square + 2, !color))
            moves.push_back(Move(source_square, source_square + 2, piece, 0, 0, 0, 0, 1));
        if (!current_occupancy.Test(source_square - 1) &&
            !current_occupancy.Test(source_square - 2) &&
            !current_occupancy.Test(source_square - 3) &&
            !IsAttacked(chess_board, source_square - 1, !color) &&
            !IsAttacked(chess_board, source_square - 2, !color))
            moves.push_back(Move(source_square, source_square - 2, piece, 0, 0, 0, 0, 1));
    }

    Bitboard king_attack_mask = AttackMask::GetAttackMask(AttackMask::King, source_square);
    while (king_attack_mask)
    {
        int target_square = king_attack_mask.PopLsbIndex();
        Bitboard color_occupancy = chess_board.GetColorOccupancy(color);
        if (!IsAttacked(chess_board, target_square, !color) && !color_occupancy.Test(target_square))
            moves.push_back(Move(source_square, target_square, piece, 0, 0, 0, 0, 0));
    }
    return moves;
}

std::vector<Move> MoveGen::GetKnightMoves(ChessBoard chess_board, int color)
{
    std::vector<Move> moves;
    int piece = color ? ChessEncoding::n : ChessEncoding::N;
    Bitboard knight_occupancy = chess_board.GetPieceOccupancy(piece);
    while (knight_occupancy)
    {
        int source_square = knight_occupancy.PopLsbIndex();
        Bitboard knight_attack_mask = AttackMask::GetAttackMask(AttackMask::Knight, source_square);
        while (knight_attack_mask)
        {
            int target_square = knight_attack_mask.PopLsbIndex();
            if (!chess_board.GetColorOccupancy(ChessEncoding::BOTH).Test(target_square))
                moves.push_back(Move(source_square, target_square, piece, 0, 0, 0, 0, 0));
            if (chess_board.GetColorOccupancy(!color).Test(target_square))
                moves.push_back(Move(source_square, target_square, piece, 0, 1, 0, 0, 0));
        }
    }
    return moves;
}

std::vector<Move> MoveGen::GetBishopMoves(ChessBoard chess_board, int color)
{
    std::vector<Move> moves;
    int piece = color ? ChessEncoding::b : ChessEncoding::B;
    Bitboard bishop_occupancy = chess_board.GetPieceOccupancy(piece);
    Bitboard current_occupancy = chess_board.GetColorOccupancy(ChessEncoding::BOTH);
    while (bishop_occupancy)
    {
        int source_square = bishop_occupancy.PopLsbIndex();
        Bitboard bishop_attack_mask = AttackMask::GetAttackMask(AttackMask::Bishop, source_square, current_occupancy);
        while (bishop_attack_mask)
        {
            int target_square = bishop_attack_mask.PopLsbIndex();
            if (!current_occupancy.Test(target_square))
                moves.push_back(Move(source_square, target_square, piece, 0, 0, 0, 0, 0));
            if (chess_board.GetColorOccupancy(!color).Test(target_square))
                moves.push_back(Move(source_square, target_square, piece, 0, 1, 0, 0, 0));
        }
    }
    return moves;
}

std::vector<Move> MoveGen::GetRookMoves(ChessBoard chess_board, int color)
{
    std::vector<Move> moves;
    int piece = color ? ChessEncoding::r : ChessEncoding::R;
    Bitboard rook_occupancy = chess_board.GetPieceOccupancy(piece);
    Bitboard current_occupancy = chess_board.GetColorOccupancy(ChessEncoding::BOTH);
    while (rook_occupancy)
    {
        int source_square = rook_occupancy.PopLsbIndex();
        Bitboard rook_attack_mask = AttackMask::GetAttackMask(AttackMask::Rook, source_square, current_occupancy);
        while (rook_attack_mask)
        {
            int target_square = rook_attack_mask.PopLsbIndex();
            if (!current_occupancy.Test(target_square))
                moves.push_back(Move(source_square, target_square, piece, 0, 0, 0, 0, 0));
            if (chess_board.GetColorOccupancy(!color).Test(target_square))
                moves.push_back(Move(source_square, target_square, piece, 0, 1, 0, 0, 0));
        }
    }
    return moves;
}
std::vector<Move> MoveGen::GetQueenMoves(ChessBoard chess_board, int color)
{
    std::vector<Move> moves;
    int piece = color ? ChessEncoding::q : ChessEncoding::Q;
    Bitboard queen_occupancy = chess_board.GetPieceOccupancy(piece);
    Bitboard current_occupancy = chess_board.GetColorOccupancy(ChessEncoding::BOTH);
    while (queen_occupancy)
    {
        int source_square = queen_occupancy.PopLsbIndex();
        Bitboard queen_attack_mask = AttackMask::GetAttackMask(AttackMask::Queen, source_square, current_occupancy);
        while (queen_attack_mask)
        {
            int target_square = queen_attack_mask.PopLsbIndex();
            ;
            if (!current_occupancy.Test(target_square))
                moves.push_back(Move(source_square, target_square, piece, 0, 0, 0, 0, 0));
            if (chess_board.GetColorOccupancy(!color).Test(target_square))
                moves.push_back(Move(source_square, target_square, piece, 0, 1, 0, 0, 0));
        }
    }
    return moves;
}

std::vector<Move> MoveGen::GetMoves(ChessBoard chess_board, int color)
{
    std::vector<Move> knight_moves = GetKnightMoves(chess_board, color);
    std::vector<Move> moves = GetPawnMoves(chess_board, color);
    std::vector<Move> king_moves = GetKingMoves(chess_board, color);
    std::vector<Move> bishop_moves = GetBishopMoves(chess_board, color);
    std::vector<Move> rook_moves = GetRookMoves(chess_board, color);
    std::vector<Move> queen_moves = GetQueenMoves(chess_board, color);
    moves.insert(moves.end(), king_moves.begin(), king_moves.end());
    moves.insert(moves.end(), knight_moves.begin(), knight_moves.end());
    moves.insert(moves.end(), bishop_moves.begin(), bishop_moves.end());
    moves.insert(moves.end(), rook_moves.begin(), rook_moves.end());
    moves.insert(moves.end(), queen_moves.begin(), queen_moves.end());
    return moves;
}

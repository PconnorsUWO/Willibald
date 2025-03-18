//
// Created by Alexander King Perocho on 2025-03-18.
//
#include "../include/controller.h"

#include <algorithm>
#include "../include/chess.h"

Controller::Controller(Chessboard& chessboard) : chessboard(chessboard)
{
}

void Controller::MakeMove(const Move& move, const int color) const
{
    int source_square = move.Decode(Move::SourceSquare);
    int target_square = move.Decode(Move::TargetSquare);
    int piece = move.Decode(Move::Piece);
    int promotion_piece = move.Decode(Move::PromotionPiece);
    int is_capture = move.Decode(Move::CaptureFlag);
    int is_double_pawn_push = move.Decode(Move::DoublePawnPushFlag);
    int is_castle = move.Decode(Move::CastleFlag);
    int is_enpassant = move.Decode(Move::EnpassantFlag);
    if (is_capture)
    {
        chessboard.color_occupancies[color].Clear(source_square);
        chessboard.piece_occupancies[piece].Clear(source_square);
        chessboard.color_occupancies[color].Set(target_square);
        chessboard.piece_occupancies[piece].Set(target_square);
        int captured_piece = color ? Chess::P : Chess::p;
        if (is_enpassant && chessboard.enpassant_square != Chess::NO_SQUARE)
        {
            /*
                I don't know if redundancy exists:
                In case of enpassant: target_square === chessboard.enpassant_square.
                Also: chessboard.enpassant_square is not the square holding the captured pawn, it's the capturing_pawns intended destination.
            */
            chessboard.piece_occupancies[captured_piece].Clear(
                color ? chessboard.enpassant_square - 8 : chessboard.enpassant_square + 8);
            chessboard.color_occupancies[!color].Clear(
                color ? chessboard.enpassant_square - 8 : chessboard.enpassant_square + 8);
        }
        else
        {
            int last_piece = captured_piece + 6;
            while (captured_piece < last_piece)
            {
                chessboard.piece_occupancies[captured_piece].Clear(target_square);
                captured_piece++;
            }
            chessboard.color_occupancies[!color].Clear(target_square);
        }
    }
}

void Controller::ParseFen(const std::string& fen) const
{
    std::fill(std::begin(chessboard.piece_occupancies), std::end(chessboard.piece_occupancies), 0ULL);
    std::fill(std::begin(chessboard.color_occupancies), std::end(chessboard.color_occupancies), 0ULL);
    chessboard.side_to_move = Chess::WHITE;
    chessboard.enpassant_square = Chess::NO_SQUARE;
    chessboard.castle_privilege = Chess::NO_CASTLE;

    int i = 0;
    for (int rank = 0; rank < 8; rank++)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            if ((fen[i] >= 'a' && fen[i] <= 'z') || (fen[i] >= 'A' && fen[i] <= 'Z'))
            {
                int piece = Chess::ASCII_INT[fen[i]];
                chessboard.piece_occupancies[piece].Set(square);
                i++;
            }
            if (fen[i] >= '0' && fen[i] <= '9')
            {
                int offset = fen[i] - '0';
                int piece = -1;
                for (int bb_piece = Chess::P; bb_piece <= Chess::k; bb_piece++)
                {
                    if (chessboard.piece_occupancies[bb_piece].Test(square))
                        piece = bb_piece;
                }
                if (piece == -1)
                    file--;
                file += offset;
                i++;
            }
            if (fen[i] == '/')
                i++;
        }
    }
    i++;
    chessboard.side_to_move = fen[i] == 'w' ? Chess::WHITE : Chess::BLACK;
    i += 2;
    while (fen[i] != ' ')
    {
        switch (fen[i])
        {
        case 'K':
            chessboard.castle_privilege |= Chess::wk;
            break;
        case 'Q':
            chessboard.castle_privilege |= Chess::wq;
            break;
        case 'k':
            chessboard.castle_privilege |= Chess::bk;
            break;
        case 'q':
            chessboard.castle_privilege |= Chess::bq;
            break;
        case '-':
            chessboard.castle_privilege |= Chess::NO_CASTLE;
            break;
        default:
            break;
        }
        i++;
    }
    i++;
    if (fen[i] != '-')
    {
        int file = fen[i] - 'a';
        int rank = 8 - (fen[i + 1] - '0');
        chessboard.enpassant_square = rank * 8 + file;
    }
    else
        chessboard.enpassant_square = Chess::NO_SQUARE;
    for (int piece = Chess::P; piece <= Chess::K; piece++)
        chessboard.color_occupancies[Chess::WHITE] |= chessboard.piece_occupancies[piece];
    for (int piece = Chess::p; piece <= Chess::k; piece++)
        chessboard.color_occupancies[Chess::BLACK] |= chessboard.piece_occupancies[piece];
    chessboard.color_occupancies[Chess::BOTH] |= chessboard.color_occupancies[Chess::WHITE];
    chessboard.color_occupancies[Chess::BOTH] |= chessboard.color_occupancies[Chess::BLACK];
}

// TODO: implement
bool Controller::IsLegalPosition(Bitboard occupancy) const { return false; }

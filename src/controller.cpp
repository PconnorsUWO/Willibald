//
// Created by Alexander King Perocho on 2025-03-18.
//
#include "../include/controller.h"
#include "../include/chess.h"
#include <algorithm>


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
    
    // Clear piece from source square and update color occupancy
    chessboard.piece_occupancies[piece].Clear(source_square);
    chessboard.color_occupancies[color].Clear(source_square);
    
    // Handle promotion (change piece type if promotion)
    if (promotion_piece) {
        // Use the color's version of the promotion piece
        int actual_promotion_piece = color ? promotion_piece + 6 : promotion_piece;
        chessboard.piece_occupancies[actual_promotion_piece].Set(target_square);
    } else {
        // Set the moved piece at target square
        chessboard.piece_occupancies[piece].Set(target_square);
    }
    
    // Update color occupancy for target square
    chessboard.color_occupancies[color].Set(target_square);
    
    // Handle capture
    if (is_capture) {
        if (is_enpassant && chessboard.enpassant_square != Chess::NO_SQUARE) {
            int captured_pawn = color ? Chess::P : Chess::p;
            int captured_square = color ? chessboard.enpassant_square - 8 : chessboard.enpassant_square + 8;
            
            chessboard.piece_occupancies[captured_pawn].Clear(captured_square);
            chessboard.color_occupancies[!color].Clear(captured_square);
            chessboard.color_occupancies[Chess::BOTH].Clear(captured_square);
        } else {
            // Clear all possible enemy pieces from target square
            int captured_piece_start = color ? Chess::P : Chess::p;
            int captured_piece_end = captured_piece_start + 6;
            
            for (int cp = captured_piece_start; cp < captured_piece_end; cp++) {
                chessboard.piece_occupancies[cp].Clear(target_square);
            }
            chessboard.color_occupancies[!color].Clear(target_square);
        }
    }
    
    // Handle castling (move the rook as well)
    if (is_castle) {
        // TODO: Add castling logic
    }
    
    // Update en passant square
    if (is_double_pawn_push) {
        chessboard.enpassant_square = color ? target_square - 8 : target_square + 8;
    } else {
        chessboard.enpassant_square = Chess::NO_SQUARE;
    }
    
    // Update combined occupancy
    chessboard.color_occupancies[Chess::BOTH] = 
        chessboard.color_occupancies[Chess::WHITE] | chessboard.color_occupancies[Chess::BLACK];
        
    // Update side to move
    chessboard.side_to_move = !color;
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

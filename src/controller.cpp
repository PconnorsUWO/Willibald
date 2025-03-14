#include "../include/controller.hpp"
#include "../include/encodings.hpp"

#include <algorithm>

Controller::Controller(ChessBoard &chess_board) : chess_board(chess_board) {}
void Controller::MakeMove(Move move, int color)
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
        chess_board.color_occupancies[color].Clear(source_square);
        chess_board.piece_occupancies[piece].Clear(source_square);
        chess_board.color_occupancies[color].Set(target_square);
        chess_board.piece_occupancies[piece].Set(target_square);
        int captured_piece = color ? ChessEncoding::P : ChessEncoding::p;
        if (is_enpassant && chess_board.enpassant_square != ChessEncoding::NO_SQUARE)
        {
            /*
                In the case of en passant, the target_square is the destination square
                of the capturing pawn and not the square holding the captured pawn.
            */
            chess_board.piece_occupancies[captured_piece].Clear(color ? chess_board.enpassant_square - 8 : chess_board.enpassant_square + 8);
            chess_board.color_occupancies[!color].Clear(color ? chess_board.enpassant_square - 8 : chess_board.enpassant_square + 8);
        }
        else
        {
            int last_piece = captured_piece + 6;
            while (captured_piece < last_piece)
            {
                chess_board.piece_occupancies[captured_piece].Clear(target_square);
                captured_piece++;
            }
            chess_board.color_occupancies[!color].Clear(target_square);
        }
    }
    else
    {

        chess_board.color_occupancies[color].Clear(source_square);
        chess_board.piece_occupancies[piece].Clear(source_square);

        if (promotion_piece != 0)
        {
            chess_board.piece_occupancies[promotion_piece].Set(target_square);
        }
        else
        {
            chess_board.piece_occupancies[piece].Set(target_square);
        }
        chess_board.color_occupancies[color].Set(target_square);
    }
}

void Controller::ParseFen(std::string fen)
{
    std::fill(std::begin(chess_board.piece_occupancies), std::end(chess_board.piece_occupancies), 0ULL);
    std::fill(std::begin(chess_board.color_occupancies), std::end(chess_board.color_occupancies), 0ULL);
    chess_board.side_to_move = ChessEncoding::WHITE;
    chess_board.enpassant_square = ChessEncoding::NO_SQUARE;
    chess_board.castle_privelage = ChessEncoding::NO_CASTLE;
    int i = 0;
    for (int rank = 0; rank < 8; rank++)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            if ((fen[i] >= 'a' && fen[i] <= 'z') || (fen[i] >= 'A' && fen[i] <= 'Z'))
            {
                int piece = ChessEncoding::ASCII_INT[fen[i]];
                chess_board.piece_occupancies[piece].Set(square);
                i++;
            }
            if (fen[i] >= '0' && fen[i] <= '9')
            {
                int offset = fen[i] - '0';
                int piece = -1;
                for (int bb_piece = ChessEncoding::P; bb_piece <= ChessEncoding::k; bb_piece++)
                {
                    if (chess_board.piece_occupancies[bb_piece].Test(square))
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
    chess_board.side_to_move = fen[i] == 'w' ? ChessEncoding::WHITE : ChessEncoding::BLACK;
    i += 2;
    while (fen[i] != ' ')
    {
        switch (fen[i])
        {
        case 'K':
            chess_board.castle_privelage |= ChessEncoding::wk;
            break;
        case 'Q':
            chess_board.castle_privelage |= ChessEncoding::wq;
            break;
        case 'k':
            chess_board.castle_privelage |= ChessEncoding::bk;
            break;
        case 'q':
            chess_board.castle_privelage |= ChessEncoding::bq;
            break;
        case '-':
            chess_board.castle_privelage |= ChessEncoding::NO_CASTLE;
            break;
        }
        i++;
    }
    i++;
    if (fen[i] != '-')
    {
        int file = fen[i] - 'a';
        int rank = 8 - (fen[i + 1] - '0');
        chess_board.enpassant_square = rank * 8 + file;
    }
    else
        chess_board.enpassant_square = ChessEncoding::NO_SQUARE;
    for (int piece = ChessEncoding::P; piece <= ChessEncoding::K; piece++)
        chess_board.color_occupancies[ChessEncoding::WHITE] |= chess_board.piece_occupancies[piece];
    for (int piece = ChessEncoding::p; piece <= ChessEncoding::k; piece++)
        chess_board.color_occupancies[ChessEncoding::BLACK] |= chess_board.piece_occupancies[piece];
    chess_board.color_occupancies[ChessEncoding::BOTH] |= chess_board.color_occupancies[ChessEncoding::WHITE];
    chess_board.color_occupancies[ChessEncoding::BOTH] |= chess_board.color_occupancies[ChessEncoding::BLACK];
}

bool Controller::IsLegalPosition(Bitboard occupancy) const { return false; }
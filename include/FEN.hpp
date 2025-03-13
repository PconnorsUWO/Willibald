#pragma once

#include <string>

namespace Fen
{
    const std::string EMPTY_BOARD = "8/8/8/8/8/8/8/8 b - - ";
    const std::string START_POSITION = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ";
    const std::string TRICKY_POSITION = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 ";
    const std::string KILLER_POSITION = "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1";
    const std::string CMK_POSITION = "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 ";
    const std::string FEN_1 = "r1bqkbnr/pppp1ppp/3n4/4N3/4P3/8/PPPP1PPP/RNBQKB1R w KQkq - 0 1";
    const std::string FEN_2 = "r1b1kb1r/ppp2ppp/2p2n2/8/3qP3/5N2/PPPP1PPP/RNBQK2R b KQkq - 2 6";
}

#include "../include/bitboard.hpp"
#include <iostream>

Bitboard::Bitboard() : value(0ULL) {}
Bitboard::Bitboard(uint64_t ull) : value(ull) {}
Bitboard::BitProxy::BitProxy(Bitboard &bitboard, int square) : bitboard(bitboard), square(square) {}
void Bitboard::Print() const
{
    for (int rank = 0; rank < 8; rank++)
    {
        std::cout << 8 - rank << "   ";
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            std::cout << Test(square) << ' ';
        }
        std::cout << ('\n');
    }
    std::cout << "\n    a b c d e f g h\n";
    std::cout << "\n decimal value = " << std::dec << value << '\n';
}
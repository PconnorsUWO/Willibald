//
// Created by Alexander King Perocho on 2025-03-10.
//

#ifndef BITBOARD_H
#define BITBOARD_H

#include <cstdint>

class Bitboard
{
public:
    Bitboard();
    explicit Bitboard(uint64_t ull);
    class BitProxy
    {
    public:
        BitProxy(Bitboard &bitboard, int square);
        inline BitProxy &operator=(bool value)
        {
            value ? bitboard.value |= (1ULL << square) : bitboard.value &= ~(1ULL << square);
            return *this;
        };
        inline explicit operator bool() const { return (bitboard.value & (1ULL << square)); } // != 0ULL

    private:
        Bitboard &bitboard;
        int square;
    };
    inline BitProxy operator[](const int square) { return BitProxy(*this, square); }
    inline explicit operator uint64_t() const { return value; }
    inline bool operator[](int square) const { return (value & (1ULL << square)) != 0; } // or (bitboard & 1ULL << (square)); ?

    inline Bitboard operator&(const uint64_t ull) const { return Bitboard(value & ull); }
    inline Bitboard operator|(const uint64_t ull) const { return Bitboard(value | ull); }
    inline Bitboard operator^(const uint64_t ull) const { return Bitboard(value ^ ull); }

    inline Bitboard operator&(const Bitboard other) const { return Bitboard(value & other.value); }
    inline Bitboard operator|(const Bitboard other) const { return Bitboard(value | other.value); }
    inline Bitboard operator^(const Bitboard other) const { return Bitboard(value ^ other.value); }

    inline Bitboard operator~() const { return Bitboard(~value); }
    inline Bitboard operator<<(int shift) const { return Bitboard(value << shift); }
    inline Bitboard operator>>(int shift) const { return Bitboard(value >> shift); }
    inline Bitboard &operator&=(const Bitboard other)
    {
        value &= other.value;
        return *this;
    }
    inline Bitboard &operator|=(const Bitboard other)
    {
        value |= other.value;
        return *this;
    }
    inline Bitboard &operator^=(const Bitboard other)
    {
        value ^= other.value;
        return *this;
    }
    inline Bitboard &operator<<=(int shift)
    {
        value <<= shift;
        return *this;
    }
    inline Bitboard &operator>>=(int shift)
    {
        value >>= shift;
        return *this;
    }
    inline Bitboard &operator*=(const Bitboard other)
    {
        value *= other.value;
        return *this;
    }
    inline bool Test(int square) const { return operator[](square); }
    inline Bitboard &Set(int square)
    {
        value |= (1ULL << square);
        return *this;
    }
    inline Bitboard &Clear(int square)
    {
        value &= ~(1ULL << square);
        return *this;
    }
    inline int GetLsbIndex() const { return value ? __builtin_ctzll(value) : -1; }
    inline int CountBits() const { return __builtin_popcountll(value); }
    inline int PopLsbIndex()
    {
        int index = GetLsbIndex();
        value &= value - 1;
        return (index < 64) ? index : -1;
    }
    void Print() const;

private:
    uint64_t value;
    friend class BitProxy;
};

#endif //BITBOARD_H

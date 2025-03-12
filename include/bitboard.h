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
    Bitboard(uint64_t ull);

    class BitProxy
    {
    public:
        BitProxy(Bitboard& bitboard, int square);

        inline BitProxy& operator=(const bool value)
        {
            value ? bitboard.value |= (1ULL << square) : bitboard.value &= ~(1ULL << square);
            return *this;
        }

        inline explicit operator bool() const { return bitboard.value != 0; } // != 0ULL

    private:
        Bitboard& bitboard;
        int square;
    };

    // Implicit Type Conversion
    inline explicit operator bool() const { return value != 0; } // != 0ULL
    inline operator int() const { return static_cast<int>(value); }
    inline explicit operator uint64_t() const { return value; }

    inline BitProxy operator[](const int square) { return {*this, square}; }
    inline bool operator[](const int square) const { return (value & (1ULL << square)) != 0; }
    // or (bitboard & 1ULL << (square)); ?

    inline Bitboard operator&(const uint64_t ull) const { return {value & ull}; }
    inline Bitboard operator|(const uint64_t ull) const { return {value | ull}; }
    inline Bitboard operator^(const uint64_t ull) const { return {value ^ ull}; }
    inline Bitboard operator*(const uint64_t ull) const { return {value * ull}; }

    inline Bitboard operator&(const Bitboard other) const { return {value & other.value}; }
    inline Bitboard operator|(const Bitboard other) const { return {value | other.value}; }
    inline Bitboard operator^(const Bitboard other) const { return {value ^ other.value}; }
    inline Bitboard operator*(const Bitboard other) const { return {value * other.value}; }


    inline Bitboard operator~() const { return {~value}; }
    inline Bitboard operator<<(const int shift) const { return {value << shift}; }
    inline Bitboard operator>>(const int shift) const { return {value >> shift}; }
    inline Bitboard operator*(const int i) const { return {value * i}; }

    inline Bitboard& operator&=(const Bitboard other)
    {
        value &= other.value;
        return *this;
    }

    inline Bitboard& operator|=(const Bitboard other)
    {
        value |= other.value;
        return *this;
    }

    inline Bitboard& operator^=(const Bitboard other)
    {
        value ^= other.value;
        return *this;
    }

    inline Bitboard& operator<<=(const int shift)
    {
        value <<= shift;
        return *this;
    }

    inline Bitboard& operator>>=(const int shift)
    {
        value >>= shift;
        return *this;
    }

    inline Bitboard& operator*=(const Bitboard other)
    {
        value *= other.value;
        return *this;
    }

    [[nodiscard]] inline bool Test(const int square) const { return operator[](square); }

    inline Bitboard& Set(const int square)
    {
        value |= (1ULL << square);
        return *this;
    }

    inline Bitboard& Clear(const int square)
    {
        value &= ~(1ULL << square);
        return *this;
    }

    [[nodiscard]] inline int GetLsbIndex() const { return value ? __builtin_ctzll(value) : -1; }
    [[nodiscard]] inline int CountBits() const { return __builtin_popcountll(value); }

    inline int PopLsbIndex()
    {
        const int index = GetLsbIndex();
        value &= value - 1;
        return (index < 64) ? index : -1;
    }

    void Print() const;

private:
    uint64_t value;
    friend class BitProxy;
};

#endif //BITBOARD_H

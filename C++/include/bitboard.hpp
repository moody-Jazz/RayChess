#pragma once

#define uint64 unsigned long long

class BitBoard{
    public:
        uint64 bitboard;

        void print_binary(); // print the board in binary form (1s and 0s)
        
        inline bool get_bit(int square);
        inline void set_bit(int square);
        inline void pop_bit(int square);
        inline void set_val(uint64 bitboard);
};
// enum definitions **********************************************************************************************

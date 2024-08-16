#pragma once
#include<vector>
#define uint64 unsigned long long

class BitBoard{
    public:
        uint64 val;
        BitBoard();
        BitBoard(uint64 val);
        void print_binary(); // print the board in binary form (1s and 0s)
        
        bool get_bit(int square);
        void set_bit(int square);
        void pop_bit(int square);
        void set_val(uint64 val);
        int count_bits();
        int get_lsb_index();
        std::vector<int> BitBoard::get_set_bit_index();
};
// enum definitions **********************************************************************************************

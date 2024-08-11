#pragma once
#include<string>
#include "../include/bitboard.hpp"
#include<unordered_map>

#define uint64 unsigned long long

class Piece{
    public:
        Piece();
        BitBoard piece_set[12];
        BitBoard get_knight_attacks(int square);
        BitBoard get_king_attacks(int square);
        BitBoard get_bishop_attacks(int square, BitBoard block);
        BitBoard get_rook_attacks(int square, BitBoard block);
        BitBoard get_queen_attacks(int square, BitBoard block);
        BitBoard get_legal_move();
};
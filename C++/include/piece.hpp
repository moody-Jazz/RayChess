#pragma once
#include<string>
#include<./include/bitboard.hpp>
#include<unordered_map>

#define uint64 unsigned long long

class Piece{
    public:
        Piece();

        enum{P,N,B,R,Q,K,p,n,b,r,q,k};
        // ASCII pieces
        const std::string ascii_pieces = "PNBRQKpnbrqk";
        // unicode pieces
        const char unicode_pieces[12] = {'P','N','B','Q','K','p','n','b','r','q','k'};
        // convert ascii char pieces to encode constants
        const std::unordered_map<char, int>char_pieces = {
            {'P', P},
            {'N', N},
            {'B', B},
            {'R', R},
            {'Q', Q},
            {'K', K},
            {'p', p},
            {'n', n},
            {'b', b},
            {'r', r},
            {'q', q},
            {'k', k}
        };
        BitBoard piece_set[12];
        BitBoard get_knight_attacks(int square);
        BitBoard get_king_attacks(int square);
        BitBoard get_bishop_attacks(int square, BitBoard block);
        BitBoard get_rook_attacks(int square, BitBoard block);
        BitBoard get_queen_attacks(int square, BitBoard block);

};
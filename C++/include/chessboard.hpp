#pragma once
#include<string>
#include "../include/bitboard.hpp"
#include <vector>
#include "../include/helper.hpp"

#define uint64 unsigned long long

static const std::string coordinate[] = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
};
/*  
not file a :
    8  0 1 1 1 1 1 1 1
    7  0 1 1 1 1 1 1 1
    6  0 1 1 1 1 1 1 1
    5  0 1 1 1 1 1 1 1
    4  0 1 1 1 1 1 1 1
    3  0 1 1 1 1 1 1 1
    2  0 1 1 1 1 1 1 1
    1  0 1 1 1 1 1 1 1
       a b c d e f g h
*/static const uint64 not_a_file = 18374403900871474942ULL;
/*  
not_h_file: 
    8  1 1 1 1 1 1 1 0
    7  1 1 1 1 1 1 1 0
    6  1 1 1 1 1 1 1 0
    5  1 1 1 1 1 1 1 0
    4  1 1 1 1 1 1 1 0
    3  1 1 1 1 1 1 1 0
    2  1 1 1 1 1 1 1 0
    1  1 1 1 1 1 1 1 0
       a b c d e f g h
*/static const uint64 not_h_file = 9187201950435737471ULL;
/*  
not_hg_file:
    8  1 1 1 1 1 1 0 0
    7  1 1 1 1 1 1 0 0
    6  1 1 1 1 1 1 0 0
    5  1 1 1 1 1 1 0 0
    4  1 1 1 1 1 1 0 0 
    3  1 1 1 1 1 1 0 0
    2  1 1 1 1 1 1 0 0
    1  1 1 1 1 1 1 0 0
       a b c d e f g h
*/static const uint64 not_hg_file = 4557430888798830399ULL;
/*  
not_ab_file:
    8  0 0 1 1 1 1 1 1
    7  0 0 1 1 1 1 1 1
    6  0 0 1 1 1 1 1 1
    5  0 0 1 1 1 1 1 1
    4  0 0 1 1 1 1 1 1
    3  0 0 1 1 1 1 1 1
    2  0 0 1 1 1 1 1 1
    1  0 0 1 1 1 1 1 1
       a b c d e f g h
*/static const uint64 not_ab_file = 18229723555195321596ULL;

class Board{
    public:
        int turn;
        bool castle[4]; 
        BitBoard bitboards[3]; // {white, black, both}
        char matrix_board[8][8];
        std::string FEN_string;
        int en_passant;
        int empty_moves;
        int total_moves;
        float bounds[4];

        Board();
        void draw();
        void update_matrix_board();
        void print();
        void sync_bitboards(BitBoard *piece_set);
        void flip_turn();
        void matrix_to_FEN();
        void highlight_tiles(const std::vector<int> &arr);
        void make_move(PieceUI *currPiece, int releasedOnTileRow, int releasedOnTileCol, BitBoard *piece_set, int *kingPosition);
};

/*
     0 0 0 0 0 0 0 0 

*/

#include "../include/chessboard.hpp"
#include "../include/globalvar.hpp"
#include <iostream>

Board::Board(){
    turn = white;
    castle = 15; // 1111 in binary
    bitboards[white].val = bitboards[black].val = bitboards[both].val = 0ULL;
    not_safe_tiles[white] = not_safe_tiles[black] = 0ULL; // every tile is safe for king to move in starting
}

void Board::print(){
    int count{};
    for(int i=63; i>=0; i--){
        if(bitboards[both].get_bit(63)){
            for(int j{}; j<totalPiece; j++){
                if(pieceTextures[j].row * 8 + pieceTextures[j].col == i){
                    std::cout<<pieceTextures[j].type<<" ";
                    break;
                }
            }
        }
        else std::cout<<".";
        count++;
        if(count == 8){
            std::cout<<std::endl;
            count = 0;
        }
    }
}

void Board::sync_bitboards(BitBoard *piece_set){
    bitboards[white].val = bitboards[black].val = 0ULL;
    for(int i{}; i<12; i++){
        if(i<6) bitboards[white].val |= piece_set[i].val;
        else  bitboards[black].val |= piece_set[i].val;
    }
    bitboards[both].val =  bitboards[white].val |  bitboards[black].val;
}

void Board::flip_turn(){
    (turn == white)?turn = black: turn = white;
}

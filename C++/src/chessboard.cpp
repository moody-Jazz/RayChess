#include "../include/chessboard.hpp"
#include "../include/globalvar.hpp"
#include <iostream>

Board::Board(){
    turn = white;
    castle = 15; // 1111 in binary
    white_board = black_board = board = 0ULL;
}

void Board::print(){
    int count{};
    for(int i=63; i>=0; i--){
        if(board.get_bit(63)){
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
    white_board.val = black_board.val = 0ULL;
    for(int i{}; i<12; i++){
        if(i<6) white_board.val |= piece_set[i].val;
        else black_board.val |= piece_set[i].val;
    }
    board.val = white_board.val | black_board.val;
}

void Board::flip_turn(){
    (turn == white)?turn = black: turn = white;
}

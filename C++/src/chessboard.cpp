#include "../include/chessboard.hpp"
#include "../include/globalvar.hpp"
#include <iostream>

Board::Board(){
    turn = white;
    castle[wk] = castle[wq] = castle[bk] = castle[bq] = true;
    bitboards[white].val = bitboards[black].val = bitboards[both].val = 0ULL;
}

Board::Board(const Board& board){
    this->turn = board.turn;
    this->castle[0] = board.castle[0];
    this->castle[1] = board.castle[1];
    this->castle[2] = board.castle[2];
    this->castle[3] = board.castle[3];
    this->bitboards[0] = board.bitboards[0];
    this->bitboards[1] = board.bitboards[1];
    this->bitboards[2] = board.bitboards[2];
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

#include "../include/chessboard.hpp"
#include "../include/globalvar.hpp"
#include <iostream>
#include <vector>

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

void Board::draw()
{
    Color light{218, 217, 233, 255},
        dark{161, 123, 185, 255},
        tileColor;
    // coloring the board
    for (int row{}; row < 8; row++)
    {
        for (int col{}; col < 8; col++)
        {
            tileColor = ((row + col) % 2 == 0) ? light : dark; // white tiles will always be on (row + col == even) position
            DrawRectangle(tileSize * col, tileSize * row, tileSize, tileSize, tileColor);
        }
    }
    for (int i{}; i < totalPiece; i++)
        DrawTexture(pieceTextures[i].texture, pieceTextures[i].col * tileSize, pieceTextures[i].row * tileSize, WHITE);
}

void Board::print(BitBoard *piece_set){
    bool empty;
    for(int tile = 63; tile>=0; tile--){ 
        empty = true;  
        for(int i = P; i<=k; i++){
            if(piece_set[i].get_bit(tile)){
                std::cout<<ascii_pieces[i]<<" ";
                empty = false;
            }
        }
        if(empty) std::cout<<". ";
        if(tile % 8 == 0) std::cout<<"\n";
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

void Board::highlight_tiles(const std::vector<int> &arr){
    for(auto &x :arr){
            int row = (63 - x) % 8;
            int col = (63 - x) / 8;
            bool occupied_tile = isThereA_Piece(col, row);
            row *= tileSize;
            col *= tileSize;
            row += tileSize/2;
            col += tileSize/2;
            Color temp = {70, 70, 70, 100};
            
            if(occupied_tile) DrawRing({(float)row, (float)col}, 
                    tileSize/2-7, tileSize/2, 0, 360, 1000, temp);

            else DrawCircle(row, col, tileSize/2-35, temp);
    }
}
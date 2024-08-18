#include <iostream>
#include "../include/inputhandler.hpp"
#include "../include/raylib.h"
#include "../include/gamesound.hpp"
#include "../include/globalvar.hpp"
#include "../include/bitboard.hpp"
#include "../include/piece.hpp"
#include <unordered_map>
#include <vector>

InputHandler::InputHandler(){
    clickedOnRow = -1;
    clickedOnCol = -1;
    int releasedOnTileRow = -1, releasedOnTileCol = -1;
    bool pieceSelected = false;
    currPiece = nullptr;
    board.sync_bitboards(piece.piece_set);
}

std::vector<int> legal_moves;

void highlightLegalMoves(){
        for(auto &x :legal_moves){
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

            else DrawCircle(row, col, tileSize/2-32, temp);
    }
}

void InputHandler::mouseInputHandler()
{
    // if a piece is bieng clicked copy it into the currPiece and change the cursor  
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        clickedOnRow = (int)GetMousePosition().y/tileSize; // this will give the row of the tile clicked
        clickedOnCol = (int)GetMousePosition().x/tileSize; // this will give the column of the tile clicked
        // std::cout<<pressedMousePosX<<" "<<pressedMousePosY<<std::endl;

        //if clicked on a tile with a piece; 
        currPiece = isThereA_Piece(clickedOnRow, clickedOnCol);
        if(currPiece != nullptr){
            pieceSelected = true;
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            // draw an outline and highlight the square bieng clicked
            Color temp = {255, 150, 84, 100};
            DrawRectangle((float)clickedOnCol * tileSize, (float)clickedOnRow * tileSize, tileSize, tileSize, temp);
            //std::cout<<currPiece->type<<std::endl;

            unsigned int source_tile = 63-(currPiece->row * 8 + currPiece->col);
            legal_moves = piece.get_legal_move(board, currPiece->type, source_tile);
            std::cout<<legal_moves.size()<<" size of legal move\n";
        }
    }
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && pieceSelected)
        draggingPiece();

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && pieceSelected)
        movedPiece();
}

void InputHandler::draggingPiece(){

    highlightLegalMoves();
   
    Rectangle rec = {
        static_cast<float>((int)GetMousePosition().x / tileSize) * tileSize,
        static_cast<float>((int)GetMousePosition().y / tileSize) * tileSize,
        tileSize,
        tileSize
    };

    // draw an outline and highlight the square bieng clicked
    DrawRectangleLinesEx(rec, 4, WHITE);
    Color temp = {255, 150, 84, 100};
    DrawRectangle((float)clickedOnCol * tileSize, (float)clickedOnRow * tileSize, tileSize, tileSize, temp);
    // std::cout <<currPiece->type<<" "<< clickedOnRow << " " << clickedOnCol << std::endl;
    // if piece is dragged redraw its textures
    DrawTexture(currPiece->texture, GetMousePosition().x - 45, GetMousePosition().y - 45, WHITE);


}

void InputHandler::movedPiece(){
    SetMouseCursor(MOUSE_CURSOR_ARROW);
    releasedOnTileCol = GetMousePosition().x/tileSize;
    releasedOnTileRow = GetMousePosition().y/tileSize;
    PieceUI *releasedOnPiece = isThereA_Piece(releasedOnTileRow,releasedOnTileCol); 

    bool isMouseInsideVertically = GetMousePosition().y >= 0 && GetMousePosition().y <= GetScreenWidth(),
            isMouseInsideHorizontally = GetMousePosition().x >= 0 && GetMousePosition().x <= GetScreenHeight(),
            isPieceReleasedOnEmptyTile = (!releasedOnPiece && (currPiece->row != releasedOnTileRow || currPiece->col != releasedOnTileCol))? true : false,
            isPieceReleasedOnEnemyTile = (!isPieceReleasedOnEmptyTile && ((releasedOnPiece->type >= 'a' && currPiece->type <= 'Z') || (releasedOnPiece->type <= 'Z' && currPiece->type >= 'a')));

    // if mouse is holding a piece and released inside the board (onto empty tile or enemy tile) update the position of the piece 
    if((isMouseInsideHorizontally && isMouseInsideVertically && pieceSelected) && (isPieceReleasedOnEmptyTile || isPieceReleasedOnEnemyTile)){
       
        /*  
        The source tile is the ith bit from which the piece moved to the destination bit.
        We are subtrating this from 63 becuase in binary we count from right to left whereas in the 
        visual board matrix we are counting from left to right hence the left top most square is at 
        0th position in visualization matrix, but for binary it is the last bit .i.e 63rd bit.
        the bitboards will be updated mimic the visual board
        */
        // pop the source tile bit and set the destinatio tile bit to update the bitboard
        unsigned int source_tile = 63-(currPiece->row * 8 + currPiece->col),
                        destination_tile = 63-(releasedOnTileRow * 8 + releasedOnTileCol);
        
        std::cout<<source_tile<<" "<<destination_tile<<" " <<currPiece->type<<std::endl;
        
        piece.piece_set[char_pieces.at(currPiece->type)].pop_bit(source_tile);
        piece.piece_set[char_pieces.at(currPiece->type)].set_bit(destination_tile);
        board.sync_bitboards(piece.piece_set);

        currPiece->row = releasedOnTileRow;
        currPiece->col = releasedOnTileCol;
        

        if(isPieceReleasedOnEmptyTile)
            sound.playDefault();  

        // if enemy piece is captured then put the captured piece at the end of the array and decrease the size till which updateBoard have acess 
        else if(isPieceReleasedOnEnemyTile){
            sound.playCapture();

            // if piece is release on enemy tile pop that enemy piece from the bitboards
            std::cout<<"captured piece: "<<releasedOnPiece->type<<std::endl;

            piece.piece_set[char_pieces.at(releasedOnPiece->type)].pop_bit(destination_tile);

            // swap the captured piece from the last piece in the pieceui array and reduce the size basically 
            // like deleting the captured piece textures
            releasedOnPiece->row = pieceTextures[totalPiece-1].row;
            releasedOnPiece->col = pieceTextures[totalPiece-1].col;
            releasedOnPiece->type = pieceTextures[totalPiece-1].type;
            releasedOnPiece->texture = pieceTextures[totalPiece-1].texture;
            totalPiece--;
        }
        // flip the turn
        board.flip_turn();
        printf((board.turn)?"black's turn\n": "white's turn\n");
    }        
    releasedOnPiece = nullptr;     
    currPiece = nullptr;
    pieceSelected = false;
}
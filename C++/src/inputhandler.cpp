#include <iostream>
#include "../include/inputhandler.hpp"
#include "../include/raylib.h"
#include "../include/gamesound.hpp"
#include "../include/globalvar.hpp"
#include "../include/bitboard.hpp"
#include "../include/piece.hpp"
#include <unordered_map>

InputHandler::InputHandler(){
    clickedOnRow = -1;
    clickedOnCol = -1;
    int releasedOnTileRow = -1, releasedOnTileCol = -1;
    bool pieceSelected = false;
    currPiece = nullptr;
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
        }
    }
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && pieceSelected)
        mouseButtonDown();

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && pieceSelected)
        mouseButtonReleased();
}

void InputHandler::mouseButtonDown(){
    
    Rectangle rec = {
        static_cast<float>((int)GetMousePosition().x / tileSize) * tileSize,
        static_cast<float>((int)GetMousePosition().y / tileSize) * tileSize,
        tileSize,
        tileSize
    };

    DrawRectangleLinesEx(rec, 4, WHITE);
    // draw an outline and highlight the square bieng clicked
    Color temp = {255, 150, 84, 100};
    DrawRectangle((float)clickedOnCol * tileSize, (float)clickedOnRow * tileSize, tileSize, tileSize, temp);
    // std::cout <<currPiece->type<<" "<< clickedOnRow << " " << clickedOnCol << std::endl;
    // if piece is dragged redraw its textures
    DrawTexture(currPiece->texture, GetMousePosition().x - 45, GetMousePosition().y - 45, WHITE);

    // highlight all the legal moves
    
}

void InputHandler::mouseButtonReleased(){
    SetMouseCursor(MOUSE_CURSOR_ARROW);
    releasedOnTileCol = GetMousePosition().x/tileSize;
    releasedOnTileRow = GetMousePosition().y/tileSize;
    PieceUI *releasedOnPiece = isThereA_Piece(releasedOnTileRow,releasedOnTileCol); 

    bool isMouseInsideVertically = GetMousePosition().y >= 0 && GetMousePosition().y <= GetScreenWidth(),
            isMouseInsideHorizontally = GetMousePosition().x >= 0 && GetMousePosition().x <= GetScreenHeight(),
            isPieceReleasedOnEmptyTile = (releasedOnPiece)? false : true,
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
        
        piece.piece_set[piece.char_pieces.at(currPiece->type)].pop_bit(source_tile);
        piece.piece_set[piece.char_pieces.at(currPiece->type)].set_bit(destination_tile);
        piece.piece_set[piece.char_pieces.at(currPiece->type)].print_binary();

        currPiece->row = releasedOnTileRow;
        currPiece->col = releasedOnTileCol;
        

        if(isPieceReleasedOnEmptyTile){    
            sound.playDefault();  
            // update the PieceUI cordinates
        }

        // if enemy piece is captured then put the captured piece at the end of the array and decrease the size till which updateBoard have acess 
        else if(isPieceReleasedOnEnemyTile){
            sound.playCapture();

            // if piece is release on enemy tile pop that enemy piece from the bitboards
            std::cout<<releasedOnPiece->type<<std::endl;
            piece.piece_set[piece.char_pieces.at(releasedOnPiece->type)].pop_bit(destination_tile);
            piece.piece_set[piece.char_pieces.at(releasedOnPiece->type)].print_binary();

            // swap the captured piece from the last piece in the pieceui array and reduce the size basically 
            // like deleting the captured piece textures
            releasedOnPiece->row = pieceTextures[totalPiece-1].row;
            releasedOnPiece->col = pieceTextures[totalPiece-1].col;
            releasedOnPiece->type = pieceTextures[totalPiece-1].type;
            releasedOnPiece->texture = pieceTextures[totalPiece-1].texture;
            totalPiece--;
        }
    }        
    releasedOnPiece = nullptr;     
    currPiece = nullptr;
    pieceSelected = false;
}
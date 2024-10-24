#include <iostream>
#include "../include/inputhandler.hpp"
#include "../include/raylib.h"
#include "../include/helper.hpp"
#include "../include/bitboard.hpp"
#include "../include/piece.hpp"
#include <unordered_map>
#include <vector>

InputHandler::InputHandler(Board &board, Piece &piece):
    board(board), piece(piece){ // Initialize references using an initializer list
    
    clickedOnRow = -1;
    clickedOnCol = -1;
    releasedOnTileRow = -1;
    releasedOnTileCol = -1;
    pieceSelected = false;
    currPiece = nullptr;
    this->board.sync_bitboards(this->piece.piece_set);
}

std::vector<int> legal_moves; // legal moves array contains the legal moves for the piece currently held by the player

/*
=======================================================================================================================================================================================================
                    Function to handle all the actions to be performed while a piece is held and bieng dragged accross the board
=======================================================================================================================================================================================================
*/

void InputHandler::mouseInputHandler()
{
    bool isMouseInsideBoard = GetMousePosition().x > board.bounds[left] && GetMousePosition().x < board.bounds[right] && 
                        GetMousePosition().y > board.bounds[top] && GetMousePosition().y < board.bounds[bottom];
    // if a piece is bieng clicked copy it into the currPiece and change the cursor  
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isMouseInsideBoard){
        clickedOnRow = ((int)GetMousePosition().y-topPadding)/tileSize; // this will give the row of the tile clicked
        clickedOnCol = ((int)GetMousePosition().x-leftPadding)/tileSize; // this will give the column of the tile clicked
        // std::cout<<pressedMousePosX<<" "<<pressedMousePosY<<std::endl;

        //if clicked on a tile with a piece; 
        currPiece = isThereA_PieceUI(clickedOnRow, clickedOnCol);
    
        if(currPiece != nullptr){
            pieceSelected = true;
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            //std::cout<<currPiece->type<<std::endl;

            unsigned int source_tile = 63-(currPiece->row * 8 + currPiece->col);

            int currTurn = (currPiece->type >= 'a')?black : white; 

            if(currTurn == board.turn) legal_moves = piece.get_legal_moves(board, currPiece->type, source_tile);
            //std::cout<<legal_moves.size()<<" is total legal moves\n";
        }
    }
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && pieceSelected)
        draggingPiece();

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && pieceSelected)
        movedPiece();
}

/*
=======================================================================================================================================================================================================
                    Function to handle all the actions to be performed while a piece is held and bieng dragged accross the board
=======================================================================================================================================================================================================
*/

void InputHandler::draggingPiece(){

    board.highlight_tiles(legal_moves);
   
   if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
        SetMouseCursor(MOUSE_CURSOR_ARROW);
        pieceSelected = false;
        legal_moves = {};
        return;
   } 

    bool isMouseInsideBoard = GetMousePosition().x > board.bounds[left] && GetMousePosition().x < board.bounds[right] && 
                        GetMousePosition().y > board.bounds[top] && GetMousePosition().y < board.bounds[bottom];
    if(isMouseInsideBoard){    
        Rectangle rec = {
            float(((int)(GetMousePosition().x-leftPadding) / tileSize) * tileSize + leftPadding),
            float(((int)(GetMousePosition().y-topPadding) / tileSize) * tileSize + topPadding), (float)tileSize, (float)tileSize
        };

        // draw an outline and highlight the square bieng clicked
        DrawRectangleLinesEx(rec, 4, WHITE);
    }
    Color temp = {255, 150, 84, 100};
    DrawRectangle(((float)clickedOnCol * tileSize)+topPadding, ((float)clickedOnRow * tileSize)+leftPadding, tileSize, tileSize, temp);
    // std::cout <<currPiece->type<<" "<< clickedOnRow << " " << clickedOnCol << std::endl;
    // if piece is dragged keep redrawing its textures
    DrawTexture(currPiece->texture, GetMousePosition().x - 45, GetMousePosition().y - 45, WHITE);
}

/*
=======================================================================================================================================================================================================
                    Function to handle all the actions to be performed when a piece is dragged across the board and released 
=======================================================================================================================================================================================================
*/
void InputHandler::movedPiece(){
    SetMouseCursor(MOUSE_CURSOR_ARROW);
    releasedOnTileCol = (GetMousePosition().x-leftPadding)/tileSize;
    releasedOnTileRow =( GetMousePosition().y-topPadding)/tileSize;
    PieceUI *releasedOnPiece = isThereA_PieceUI(releasedOnTileRow,releasedOnTileCol); 

    bool isMouseInsideBoard = GetMousePosition().y > board.bounds[top] && GetMousePosition().y <= board.bounds[bottom] && 
                                    GetMousePosition().x > board.bounds[left] && GetMousePosition().x < board.bounds[right],
            isPieceReleasedOnEmptyTile = (!releasedOnPiece && (currPiece->row != releasedOnTileRow || currPiece->col != releasedOnTileCol))? true : false,
            isPieceReleasedOnEnemyTile = (!isPieceReleasedOnEmptyTile && 
            ((releasedOnPiece->type >= 'a' && currPiece->type <= 'Z') || (releasedOnPiece->type <= 'Z' && currPiece->type >= 'a')));

    /*  
    The source tile is the ith bit from which the piece moved to the destination bit.
    We are subtrating this from 63 becuase in binary we count from right to left whereas in the 
    visual board matrix we are counting from left to right hence the left top most square is at 
    0th position in visualization matrix, but for binary it is the last bit .i.e 63rd bit.
    the bitboards will be updated mimic the visual board
    */
    // pop the source tile bit and set the destinatio tile bit to update the bitboard
    unsigned int destination_tile = 63-(releasedOnTileRow * 8 + releasedOnTileCol);

    bool isMoveLegal = false;
    for(auto &x: legal_moves){
        if((int)destination_tile == x){
            isMoveLegal = true;
            break;
        }
    }
    legal_moves = {};
    // if mouse is holding a piece and released inside the board (onto empty tile or enemy tile) update the position of the piece 
    if((isMouseInsideBoard && isMoveLegal && pieceSelected) && (isPieceReleasedOnEmptyTile || isPieceReleasedOnEnemyTile)){
        board.en_passant = -1;
        
        board.make_move(currPiece, releasedOnTileRow, releasedOnTileCol, piece.piece_set, piece.kingPosition);

        piece.update_unsafe_tiles(board);

        // flip the turn
        board.flip_turn();
        
        //printf((board.turn)?"black's turn\n": "white's turn\n");
    }        
    
    releasedOnPiece = nullptr;     
    currPiece = nullptr;
    pieceSelected = false;
}
#include <iostream>
#include "../include/inputhandler.hpp"
#include "../include/raylib.h"
#include "../include/helper.hpp"
#include "../include/bitboard.hpp"
#include "../include/piece.hpp"

InputHandler::InputHandler(Board &board, Piece &piece):
    board(board), piece(piece){ // Initialize references using an initializer list
    clickedOnRow = -1;
    clickedOnCol = -1;
    releasedOnTileRow = -1;
    releasedOnTileCol = -1;
    pieceSelected = false;
    currPiece = nullptr;
    this->board.syncBitboards(this->piece.pieceBitboards);
}

void InputHandler::mouseInputHandler()
{
    bool isMouseInsideBoard = GetMousePosition().x > board.padding[left] && GetMousePosition().x < board.padding[right] && 
                        GetMousePosition().y > board.padding[top] && GetMousePosition().y < board.padding[bottom];
    // if a piece is bieng clicked copy it into the currPiece and change the cursor  
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isMouseInsideBoard){
        clickedOnRow = ((int)GetMousePosition().y-Globals::topPadding)/Globals::tileSize; // this will give the row of the tile clicked
        clickedOnCol = ((int)GetMousePosition().x-Globals::leftPadding)/Globals::tileSize; // this will give the column of the tile clicked
        // std::cout<<pressedMousePosX<<" "<<pressedMousePosY<<std::endl;

        //if clicked on a tile with a piece; 
        currPiece = isThereAPieceTexture(clickedOnRow, clickedOnCol);
    
        if(currPiece != nullptr){
            pieceSelected = true;
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            //std::cout<<currPiece->type<<std::endl;

            size_t sourceTile = 63-(currPiece->row * 8 + currPiece->col);

            size_t currTurn = (currPiece->type >= 'a')?black : white; 

            // if a valid piece is clicked then insert its corrosponding legal moves into the bitboard
            if(currTurn == board.turn) board.legalMoves.setVal(piece.getLegalMoves(board, currPiece->type, sourceTile));
        }
    }
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && pieceSelected)
        draggingPiece();

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && pieceSelected)
        movedPiece();
}

void InputHandler::draggingPiece(){
   
   if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
        SetMouseCursor(MOUSE_CURSOR_ARROW);
        pieceSelected = false;
        board.legalMoves.setVal(0ULL);
        return;
   } 

    bool isMouseInsideBoard = GetMousePosition().x > board.padding[left] && GetMousePosition().x < board.padding[right] && 
                        GetMousePosition().y > board.padding[top] && GetMousePosition().y < board.padding[bottom];
    if(isMouseInsideBoard){    
        Rectangle rec = {
            float(((int)(GetMousePosition().x-Globals::leftPadding) / Globals::tileSize) * Globals::tileSize + Globals::leftPadding),
            float(((int)(GetMousePosition().y-Globals::topPadding) / Globals::tileSize) * Globals::tileSize + Globals::topPadding), (float)Globals::tileSize, (float)Globals::tileSize
        };

        // draw an outline and highlight the square bieng clicked
        DrawRectangleLinesEx(rec, 4, WHITE);
    }
    Color temp = {255, 150, 84, 100};
    DrawRectangle(((float)clickedOnCol * Globals::tileSize)+Globals::topPadding, ((float)clickedOnRow * Globals::tileSize)+Globals::leftPadding, Globals::tileSize, Globals::tileSize, temp);
    // std::cout <<currPiece->type<<" "<< clickedOnRow << " " << clickedOnCol << std::endl;
    // if piece is dragged keep redrawing its textures
    DrawTexture(currPiece->texture, GetMousePosition().x - 45, GetMousePosition().y - 45, WHITE);
}

void InputHandler::movedPiece(){
    SetMouseCursor(MOUSE_CURSOR_ARROW);
    releasedOnTileCol = (GetMousePosition().x-Globals::leftPadding)/Globals::tileSize;
    releasedOnTileRow =( GetMousePosition().y-Globals::topPadding)/Globals::tileSize;
    PieceUI *releasedOnPiece = isThereAPieceTexture(releasedOnTileRow,releasedOnTileCol); 

    bool isMouseInsideBoard = GetMousePosition().y > board.padding[top] && GetMousePosition().y <= board.padding[bottom] && 
                                    GetMousePosition().x > board.padding[left] && GetMousePosition().x < board.padding[right],
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
    size_t destTile = 63-(releasedOnTileRow * 8 + releasedOnTileCol);

    bool isMoveLegal = (board.legalMoves.getVal() & (1ULL << destTile));
    
    // if mouse is holding a piece and released inside the board (onto empty tile or enemy tile) update the position of the piece 
    if((isMouseInsideBoard && isMoveLegal && pieceSelected) && (isPieceReleasedOnEmptyTile || isPieceReleasedOnEnemyTile)){
        board.enPassant = -1;
        
        board.makeMove(currPiece, releasedOnTileRow, releasedOnTileCol, piece.pieceBitboards, piece.kingPosition);

        piece.updateUnsafeTiles(board);

        // flip the turn
        board.flipTurn();
        //printf((board.turn)?"black's turn\n": "white's turn\n");
    }        
    board.legalMoves.setVal(0ULL);
    releasedOnPiece = nullptr;     
    currPiece = nullptr;
    pieceSelected = false;
}
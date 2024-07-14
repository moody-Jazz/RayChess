#include "../include/inputhandler.hpp"
#include "../include/raylib.h"
#include "../include/gamesound.hpp"
#include "../include/globalvar.hpp"

InputHandler::InputHandler(){
    clickedOnRow = -1;
    clickedOnCol = -1;
    int releasedOnTileRow = -1, releasedOnTileCol = -1;
    bool pieceSelected = false;
    Piece* currPiece = nullptr;
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
    {
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
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && pieceSelected)
    {
        SetMouseCursor(MOUSE_CURSOR_ARROW);
        releasedOnTileCol = GetMousePosition().x/tileSize;
        releasedOnTileRow = GetMousePosition().y/tileSize;
        Piece *releasedOnPiece = isThereA_Piece(releasedOnTileRow,releasedOnTileCol); 

        bool isMouseInsideVertically = GetMousePosition().y >= 0 && GetMousePosition().y <= GetScreenWidth(),
             isMouseInsideHorizontally = GetMousePosition().x >= 0 && GetMousePosition().x <= GetScreenHeight(),
             isPieceReleasedOnEmptyTile = (releasedOnPiece)? false : true,
             isPieceReleasedOnEnemyTile = (!isPieceReleasedOnEmptyTile && ((releasedOnPiece->type >= 'a' && currPiece->type <= 'Z') || (releasedOnPiece->type <= 'Z' && currPiece->type >= 'a')));

        // if mouse is holding a piece and released inside the board update the position of the piece 
        if(isMouseInsideHorizontally && isMouseInsideVertically && pieceSelected){

            if(isPieceReleasedOnEmptyTile){    
                sound.playDefault();
                currPiece->row = releasedOnTileRow;
                currPiece->col = releasedOnTileCol;
            }

            // if enemy piece is captured then put the captured piece at the end of the array and decrease the size till which updateBoard have acess 
            else if(isPieceReleasedOnEnemyTile){
                sound.playCapture();
                currPiece->row = releasedOnTileRow;
                currPiece->col = releasedOnTileCol;
                releasedOnPiece->row = piece[totalPiece-1].row;
                releasedOnPiece->col = piece[totalPiece-1].col;
                releasedOnPiece->type = piece[totalPiece-1].type;
                releasedOnPiece->texture = piece[totalPiece-1].texture;
                totalPiece--;
            }
        }        
        releasedOnPiece = nullptr;     
        currPiece = nullptr;
        pieceSelected = false;
    }
}
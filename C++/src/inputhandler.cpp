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

            else DrawCircle(row, col, tileSize/2-35, temp);
    }
}

/*
=======================================================================================================================================================================================================
                    Function to handle all the actions to be performed while a piece is held and bieng dragged accross the board
=======================================================================================================================================================================================================
*/

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
            legal_moves = piece.get_legal_move(board, currPiece->type, source_tile).get_set_bit_index();
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
    
    highlightLegalMoves();
   
   if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
        SetMouseCursor(MOUSE_CURSOR_ARROW);
        pieceSelected = false;
        return;
   } 


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

/*
=======================================================================================================================================================================================================
                    Function to handle all the actions to be performed when a piece is dragged across the board and released 
=======================================================================================================================================================================================================
*/
void InputHandler::movedPiece(){
    SetMouseCursor(MOUSE_CURSOR_ARROW);
    releasedOnTileCol = GetMousePosition().x/tileSize;
    releasedOnTileRow = GetMousePosition().y/tileSize;
    PieceUI *releasedOnPiece = isThereA_Piece(releasedOnTileRow,releasedOnTileCol); 

    bool isMouseInsideWindow = (GetMousePosition().y >= 0 && GetMousePosition().y <= GetScreenWidth()) && (GetMousePosition().x >= 0 && GetMousePosition().x <= GetScreenHeight()),
            isPieceReleasedOnEmptyTile = (!releasedOnPiece && (currPiece->row != releasedOnTileRow || currPiece->col != releasedOnTileCol))? true : false,
            isPieceReleasedOnEnemyTile = (!isPieceReleasedOnEmptyTile && ((releasedOnPiece->type >= 'a' && currPiece->type <= 'Z') || (releasedOnPiece->type <= 'Z' && currPiece->type >= 'a')));

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

    bool isMoveLegal = false;
    for(auto &x: legal_moves){
        if(destination_tile == x){
            isMoveLegal = true;
            break;
        }
    }
    // if mouse is holding a piece and released inside the board (onto empty tile or enemy tile) update the position of the piece 
    if((isMouseInsideWindow && isMoveLegal && pieceSelected) && (isPieceReleasedOnEmptyTile || isPieceReleasedOnEnemyTile)){

        // below if else conditions are for checking if the king or rooks have moved to update the casteling state ont he board
        if(currPiece->type == 'K'){

            piece.kingPosition[white] = 63 - (releasedOnTileRow*8+releasedOnTileCol); //update the global white king position
            
            if(board.castle[wk] || board.castle[wq]){
                board.castle[wk] = false;
                board.castle[wq] = false;
                std::cout<<"White king can't castle now\n";
            }
        }
        else if(currPiece->type == 'k'){

            piece.kingPosition[black] =  63 - (releasedOnTileRow*8+releasedOnTileCol); //update the global black king positions

            if(board.castle[bk] || board.castle[bq]){
                board.castle[bk] = false;
                board.castle[bq] = false;
                std::cout<<"Black king can't castle now\n";
            }
        }
        if((board.castle[wk] || board.castle[wq]) && currPiece->type == 'R'){
            if(currPiece->row == 7 && currPiece->col == 7){
                board.castle[wk] = false;
                std::cout<<"White king can't castle king side now\n";
            }
            else if (currPiece->row == 7 && currPiece->col == 0){
                board.castle[wq] = false;
                std::cout<<"White king can't castle queen side now\n";
            }
        }
        else if((board.castle[bk] || board.castle[bq]) && currPiece->type == 'r'){
            if(currPiece->row == 0 && currPiece->col == 7){
                board.castle[bk] = false;
                std::cout<<"black king can't castle king side now\n";
            }
            else if (currPiece->row == 0 && currPiece->col == 0){
                board.castle[bq] = false;
                std::cout<<"black king can't castle queen side now\n";
            }
        }

        piece.piece_set[char_pieces.at(currPiece->type)].pop_bit(source_tile);
        piece.piece_set[char_pieces.at(currPiece->type)].set_bit(destination_tile);
        std::cout<<source_tile<<" "<<destination_tile<<" " <<currPiece->type<<std::endl;


        currPiece->row = releasedOnTileRow;
        currPiece->col = releasedOnTileCol;


        // if enemy piece is captured then put the captured piece at the end of the array and decrease the size till which updateBoard have acess 
        if(isPieceReleasedOnEnemyTile){
            sound.playCapture();

            // if piece is release on enemy tile pop that enemy piece from the bitboards
            piece.piece_set[char_pieces.at(releasedOnPiece->type)].pop_bit(destination_tile);

            //std::cout<<"captured piece: "<<releasedOnPiece->type<<std::endl;

            // swap the captured piece from the last piece in the pieceui array and reduce the size basically 
            // like deleting the captured piece textures updating the casteling state

            // below if else conditions are for checking if the rooks have been captured to update the casteling state ont he board
            if((board.castle[wk] || board.castle[wq]) && releasedOnPiece->type == 'R'){
                if(releasedOnPiece->row == 7 && releasedOnPiece->col == 7){
                    board.castle[wk] = false;
                    std::cout<<"White king can't castle king side now\n";
                } 
                else if (releasedOnPiece->row == 7 && releasedOnPiece->col == 0){  
                    board.castle[wq] = false;
                    std::cout<<"White king can't castle queen side now\n";
                }
            }
            else if((board.castle[bk] || board.castle[bq]) && releasedOnPiece->type == 'r'){
                if(releasedOnPiece->row == 0 && releasedOnPiece->col == 7)  {
                    board.castle[bk] = false;
                    std::cout<<"black king can't castle king side now\n";
                }
                else if (releasedOnPiece->row == 0 && releasedOnPiece->col == 0)  {
                    board.castle[bq] = false;
                    std::cout<<"black king can't castle queen side now\n";
                }
            }

            releasedOnPiece->row = pieceTextures[totalPiece-1].row;
            releasedOnPiece->col = pieceTextures[totalPiece-1].col;
            releasedOnPiece->type = pieceTextures[totalPiece-1].type;
            releasedOnPiece->texture = pieceTextures[totalPiece-1].texture;
            totalPiece--;
        }
        else if(isPieceReleasedOnEmptyTile) sound.playDefault();
 
        // check if king is in check
        uint64 temp = piece.get_legal_move(board, currPiece->type, destination_tile).val;
        int king = board.turn?K:k;
        if(temp & piece.piece_set[king].val){
            sound.playCheck();
            piece.check[!board.turn] = true;

        }

        std::cout<<"curr king is safe: "<<piece.is_king_safe(board, board.turn)<<std::endl;
        // sync the board to ensure that all the 3 bitboard in the chessboard are also updated
        board.sync_bitboards(piece.piece_set);
 
        // flip the turn
        board.flip_turn();
        
        //printf((board.turn)?"black's turn\n": "white's turn\n");
    }        
    
    releasedOnPiece = nullptr;     
    currPiece = nullptr;
    pieceSelected = false;
}
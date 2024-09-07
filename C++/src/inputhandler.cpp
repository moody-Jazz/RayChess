#include <iostream>
#include "../include/inputhandler.hpp"
#include "../include/raylib.h"
#include "../include/gamesound.hpp"
#include "../include/globalvar.hpp"
#include "../include/bitboard.hpp"
#include "../include/piece.hpp"
#include <unordered_map>
#include <vector>

InputHandler::InputHandler(Board &board, Piece &piece):
    board(board), piece(piece){ // Initialize references using an initializer list
    clickedOnRow = -1;
    clickedOnCol = -1;
    int releasedOnTileRow = -1, releasedOnTileCol = -1;
    bool pieceSelected = false;
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
            legal_moves = piece.get_legal_moves(board, currPiece->type, source_tile);
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

    bool isMouseInsideWindow = (GetMousePosition().y >= 0 && GetMousePosition().y <= GetScreenWidth()) && 
                                    (GetMousePosition().x >= 0 && GetMousePosition().x <= GetScreenHeight()),
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
        // if white king is moved
        if(currPiece->type == 'K'){

            piece.kingPosition[white] = destination_tile; //update the global white king position
            if(source_tile == 3 && destination_tile == 1){
                PieceUI* rook = isThereA_Piece(7,7);
                piece.piece_set[char_pieces.at(rook->type)].pop_bit(63-(rook->row*8+rook->col)); // eraset the old position of this rook in bitboards
                rook->col = 5;
                piece.piece_set[char_pieces.at(rook->type)].set_bit(63-(rook->row*8+rook->col)); // update the new position of this rook in bitboards
                sound.playCastle();
            }
            else if(source_tile == 3 && destination_tile == 5){
                PieceUI* rook = isThereA_Piece(7,0);
                piece.piece_set[char_pieces.at(rook->type)].pop_bit(63-(rook->row*8+rook->col)); // eraset the old position of this rook in bitboards
                rook->col = 3;
                piece.piece_set[char_pieces.at(rook->type)].set_bit(63-(rook->row*8+rook->col)); // update the new position of this rook in bitboards
                sound.playCastle();
            }

            if(board.castle[wk] || board.castle[wq]){
                board.castle[wk] = false;
                board.castle[wq] = false;
                std::cout<<"White king can't castle now\n";
            }
        }

        // if black king is moved
        else if(currPiece->type == 'k'){

            piece.kingPosition[black] =  destination_tile; //update the global black king positions

            if(source_tile == 59 && destination_tile == 57){
                PieceUI* rook = isThereA_Piece(0,7);
                piece.piece_set[char_pieces.at(rook->type)].pop_bit(63-(rook->row*8+rook->col)); // eraset the old position of this rook in bitboards
                rook->col = 5;
                piece.piece_set[char_pieces.at(rook->type)].set_bit(63-(rook->row*8+rook->col)); // update the new position of this rook in bitboards
                sound.playCastle();
            }
            else if(source_tile == 59 && destination_tile == 61){
                PieceUI* rook = isThereA_Piece(0,0);
                piece.piece_set[char_pieces.at(rook->type)].pop_bit(63-(rook->row*8+rook->col)); // eraset the old position of this rook in bitboards
                rook->col = 3;
                piece.piece_set[char_pieces.at(rook->type)].set_bit(63-(rook->row*8+rook->col)); // update the new position of this rook in bitboards
                sound.playCastle();
            }

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
        
        // sync the board to ensure that all the 3 bitboard in the chessboard are also updated
        board.sync_bitboards(piece.piece_set);

        piece.update_unsafe_tiles(board);

        // flip the turn
        board.flip_turn();
        
        //printf((board.turn)?"black's turn\n": "white's turn\n");
    }        
    
    releasedOnPiece = nullptr;     
    currPiece = nullptr;
    pieceSelected = false;
}
#include "raylib.h"
#include "gamesound.hpp" 
#include "globalvar.hpp"
#include "../include/piece.hpp"
#include "../include/chessboard.hpp"

class InputHandler
{
private:
    int clickedOnRow;
    int clickedOnCol;;
    int releasedOnTileRow;
    int releasedOnTileCol;
    bool pieceSelected;
    // pieceUi is for the visual images fo thier pieces and piece is for the bitobards
    PieceUI* currPiece;
    Board &board;
    // piece is for updating the bitboard of every piece and board
    Piece &piece;

    GameSound sound;
public:
    InputHandler(Board &board, Piece &piece);
    void mouseInputHandler();
    void draggingPiece();
    void movedPiece();
};


 



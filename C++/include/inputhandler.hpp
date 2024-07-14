#include "raylib.h"
#include "gamesound.hpp" 
#include "globalvar.hpp"

class InputHandler
{
private:
    int clickedOnRow;
    int clickedOnCol;;
    int releasedOnTileRow;
    int releasedOnTileCol;
    bool pieceSelected;
    Piece* currPiece;
    GameSound sound;
public:
    InputHandler();
    void mouseInputHandler();
};


 



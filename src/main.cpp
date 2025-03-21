#include "interface.hpp"
#include <iostream>
#include "magic.hpp"

int main()
{
    initRookBishopLookupTable();
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(0,0,"Ray Chess");
    Globals::setUIparams();
    SetWindowSize(Globals::windowWidth, Globals::windowHeight);
    centerWindow();
    SetWindowIcon(Globals::icon);
    SetTargetFPS(60);
    loadPieceTextures();
    
    Board board;
    Engine engine;
    Interface interface(board, engine);
    
    while (!WindowShouldClose())
    {  
        if(IsWindowResized())
        {   
            interface.setUIparams();
            centerWindow();
        }
        BeginDrawing();
        ClearBackground(Colors::background);
        board.drawBoardAndPieces();
        interface.drawSidePanel();
        interface.interactionHandler();
        EndDrawing();
    }
    CloseWindow();
    UnloadImage(Globals::icon);
}

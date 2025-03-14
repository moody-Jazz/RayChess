#include "interface.hpp"
#include <iostream>

int main()
{
    InitWindow(Globals::windowWidth, Globals::windowHeight, "Ray Chess");
    SetWindowIcon(Globals::icon);
    SetTargetFPS(60);
    loadPieceTextures();
    Board board;
    Engine engine;
    Interface interface(board, engine);
    
    while (!WindowShouldClose())
    {  
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

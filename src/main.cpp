#include "interface.hpp"
#include <iostream>

int main()
{
    std::cout<<"Success"<<std::endl;
    std::string logo = "RAY CHESS";
    InitWindow(Globals::windowWidth, Globals::windowHeight, logo.c_str());
    SetTargetFPS(60);
    loadPieceTextures();
    Board board;
    Engine engine;
    Interface interface(board, engine);

    //print the intial position of every piece
    // for (int i{}; i < 32; i++) 
    // {
    //     std::cout << piece[i].type << " " << piece[i].row << " " << piece[i].col << std::endl;
    // }
    
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
}

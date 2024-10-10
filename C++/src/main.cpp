#include "../include/raylib.h"
#include "../include/InputHandler.hpp"
#include "../include/helper.hpp"
#include "../include/chessboard.hpp"
#include "../include/piece.hpp"
#include <string>
#include <iostream>

void loadTextures()
{
    std::string pieceDir[] = {"../../Assets/images/blackRook.png", "../../Assets/images/blackKnight.png", "../../Assets/images/blackBishop.png", "../../Assets/images/blackQueen.png", 
                              "../../Assets/images/blackKing.png", "../../Assets/images/blackPawn.png", "../../Assets/images/whitePawn.png", "../../Assets/images/whiteRook.png", 
                              "../../Assets/images/whiteKnight.png", "../../Assets/images/whiteBishop.png", "../../Assets/images/whiteQueen.png", "../../Assets/images/whiteKing.png"};
    Texture2D pieceTexture[12]; //[br, bk, bb, bq, bking, bpawn, wpawn, wr, wk, wb, wq, wking]

    for (int i{}; i < 12; i++)
    {
        Image image = LoadImage(pieceDir[i].c_str());
        ImageResize(&image, tileSize, tileSize);
        pieceTexture[i] = LoadTextureFromImage(image); // Load texture in graphics VRAM
        UnloadImage(image);                            // unload image from RAM
    }
    InitializePieces(pieceTexture);
}
    
int main()
{
    std::cout<<"Success"<<std::endl;

    InitWindow(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()), "Chess");
    SetTargetFPS(60);
    loadTextures();
    Board board;
    Piece piece;
    InputHandler inputHandler(board, piece);

    //print the intial position of every piece
    // for (int i{}; i < 32; i++) 
    // {
    //     std::cout << piece[i].type << " " << piece[i].row << " " << piece[i].col << std::endl;
    // }
    
    while (!WindowShouldClose())
    {  
        BeginDrawing();
        ClearBackground(RAYWHITE);
        board.draw();;
        inputHandler.mouseInputHandler();
        EndDrawing();
    }
    CloseWindow();

    ///////////////////////////////////// testing /////////////////////////////////////
}

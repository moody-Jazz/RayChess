
#include "inputhandler.hpp"
#include <iostream>

void loadTextures()
{
    std::string pieceDir[] = {"images/blackRook.png", "images/blackKnight.png", "images/blackBishop.png", "images/blackQueen.png", 
                              "images/blackKing.png", "images/blackPawn.png", "images/whitePawn.png", "images/whiteRook.png", 
                              "images/whiteKnight.png", "images/whiteBishop.png", "images/whiteQueen.png", "images/whiteKing.png"};
    Texture2D pieceTexture[12]; //[br, bk, bb, bq, bking, bpawn, wpawn, wr, wk, wb, wq, wking]

    for (int i{}; i < 12; i++)
    {
        Image image = LoadImage(pieceDir[i].c_str());
        ImageResize(&image, Globals::tileSize, Globals::tileSize);
        pieceTexture[i] = LoadTextureFromImage(image); // Load texture in graphics VRAM
        UnloadImage(image);                            // unload image from RAM
    }
    InitializePieces(pieceTexture);
}
    
int main()
{
    std::cout<<"Success"<<std::endl;
    std::string logo = "RAY CHESS";
    InitWindow(1700, 1000, logo.c_str());
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
        ClearBackground(Globals::bg);
        DrawText(logo.c_str(), (Globals::tileSize*8) + Globals::leftPadding*3, Globals::tileSize/2, 100,RAYWHITE);
        board.drawUpdatedBoard();
        inputHandler.mouseInputHandler();
        EndDrawing();
    }
    CloseWindow();
}

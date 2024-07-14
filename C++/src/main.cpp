#include "../include/raylib.h"
#include "../include/InputHandler.hpp"
#include "../include/globalvar.hpp"
#include <string>
#include <iostream>

void loadTextures()
{
    std::string pieceDir[] = {"../Assets/images/blackRook.png", "../Assets/images/blackKnight.png", "../Assets/images/blackBishop.png", "../Assets/images/blackQueen.png", "../Assets/images/blackKing.png", "../Assets/images/blackPawn.png",
                              "../Assets/images/whitePawn.png", "../Assets/images/whiteRook.png", "../Assets/images/whiteKnight.png", "../Assets/images/whiteBishop.png", "../Assets/images/whiteQueen.png", "../Assets/images/whiteKing.png"};
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
void updateBoard()
{
    Color light{218, 217, 233, 255},
        dark{161, 123, 185, 255},
        tileColor;
    // coloring the board
    for (int row{}; row < 8; row++)
    {
        for (int col{}; col < 8; col++)
        {
            tileColor = ((row + col) % 2 == 0) ? light : dark; // white tiles will always be on (row + col == even) position
            DrawRectangle(tileSize * col, tileSize * row, tileSize, tileSize, tileColor);
        }
    }
    for (int i{}; i < totalPiece; i++)
    {
        DrawTexture(piece[i].texture, piece[i].col * tileSize, piece[i].row * tileSize, WHITE);
    }
} 
int main()
{
    InitWindow(tileSize * 8, tileSize * 8, "Chess");
    SetTargetFPS(60);
    loadTextures();
    InputHandler inputHandler;

    //print the intial position of every piece
    // for (int i{}; i < 32; i++) 
    // {
    //     std::cout << piece[i].type << " " << piece[i].row << " " << piece[i].col << std::endl;
    // }
    
    while (!WindowShouldClose())
    {  
        BeginDrawing();
        ClearBackground(RAYWHITE);
        updateBoard();;
        inputHandler.mouseInputHandler();
        EndDrawing();
    }
    CloseWindow();
}
#include "include/raylib.h"
#include <string>
const unsigned int tileSize = 120;

void drawBoard(){
    Color light{218,217,233,255},
          dark{161,123,185,255},
          tileColor;  

    //coloring the board
    for(int row{}; row<8; row++){
            for(int col{}; col<8; col++){ 
                tileColor = ((row + col) % 2 == 0) ? light : dark; // white tiles will always be on (row + col == even) position
                DrawRectangle(tileSize*col, tileSize*row, tileSize, tileSize, tileColor);
        }
    }

    std::string pieceDir[] = {"../Assets/blackRook.png", "../Assets/blackKnight.png", "../Assets/blackBishop.png", "../Assets/blackQueen.png", "../Assets/blackKing.png", "../Assets/blackPawn.png",
                              "../Assets/whitePawn.png", "../Assets/whiteRook.png", "../Assets/whiteKnight.png", "../Assets/whiteBishop.png", "../Assets/whiteQueen.png", "../Assets/whiteKing.png"};
    Texture2D pieceTexture[12]; //[br, bk, bb, bq, bking, bpawn, wpawn, wr, wk, wb, wq, wking]
    for(int i{}; i<12; i++){
        Image image = LoadImage(pieceDir[i].c_str());
        ImageResize(&image, tileSize, tileSize);
        pieceTexture[i] = LoadTextureFromImage(image); // Load texture in graphics VRAM
        UnloadImage(image); // unload image from RAM
    }

    // placing the pawns onto their tiles 
    for(int i{}; i<8; i++){
        DrawTexture(pieceTexture[5], tileSize*i, tileSize, WHITE);
        DrawTexture(pieceTexture[6], tileSize*i, tileSize*6, WHITE);
    }
    // placing the pieces black [r k b q king] and white [R KB Q KING] pieces from left
    for(int i{}; i<5; i++){
        DrawTexture(pieceTexture[i], tileSize*i, 0, WHITE);
        DrawTexture(pieceTexture[i+7], tileSize*i, tileSize*7, WHITE);
    }
    // placing the remaining pieces black [b k r] and white [B K R] pieces from right
    for(int i = 0; i<3; i++){
        DrawTexture(pieceTexture[i], tileSize* (7-i), 0, WHITE);
        DrawTexture(pieceTexture[i+7], tileSize* (7-i), tileSize*7, WHITE);
    }
}

int main(){
    InitWindow(tileSize*8, tileSize*8,"Chess");
    SetTargetFPS(60);

    while(!WindowShouldClose()){
        BeginDrawing();
            drawBoard();
        EndDrawing();
    }
    CloseWindow();
}
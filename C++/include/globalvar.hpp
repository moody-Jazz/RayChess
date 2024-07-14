#pragma once
#include "raylib.h"

extern int tileSize;

typedef struct Piece
{
    char type;
    Texture2D texture;
    int row;
    int col;
} Piece;

extern Piece piece[32];

extern int totalPiece;

void InitializePiece(Piece *piece, char type, Texture2D texture, int row, int col);

void InitializePieces(Texture2D texture[]);

Piece* isThereA_Piece(int x, int y);
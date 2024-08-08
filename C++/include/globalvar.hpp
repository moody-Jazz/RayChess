#pragma once
#include "raylib.h"

// this header file contains all the necessary stuff related to ui managment like piece textures
extern int tileSize;

typedef struct PieceUI
{
    char type;
    Texture2D texture;
    int row;
    int col;
} PieceUI;

extern PieceUI pieceTextures[32];

extern int totalPiece;

void InitializePiece(PieceUI *piece, char type, Texture2D texture, int row, int col);

void InitializePieces(Texture2D texture[]);

PieceUI* isThereA_Piece(int x, int y);
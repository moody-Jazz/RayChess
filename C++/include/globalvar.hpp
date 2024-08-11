#pragma once
#include "raylib.h"
#include <unordered_map>
#include <string>

// this header file contains all the necessary stuff related to ui managment like piece textures
extern int tileSize;
enum{P,N,B,R,Q,K,p,n,b,r,q,k};
// ASCII pieces
extern const std::string ascii_pieces;
// unicode pieces
extern const char unicode_pieces[12];
// convert ascii char pieces to encode constants
extern const std::unordered_map<char, int>char_pieces;

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
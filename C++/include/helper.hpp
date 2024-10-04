#pragma once
#include "raylib.h"
#include <unordered_map>
#include <string>
#include "../include/gamesound.hpp"

// this header file contains all the necessary stuff related to ui managment like piece textures
extern int tileSize;
extern Color light, dark;

enum{white, black, both};
enum{P,N,B,R,Q,K,p,n,b,r,q,k};
enum{ wk, wq, bk, bq};
// ASCII pieces
extern const std::string ascii_pieces;
// convert ascii char pieces to encode constants
extern const std::unordered_map<char, int>char_pieces;

extern GameSound sound;
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

void deletePiece(PieceUI *piece);

PieceUI* isThereA_Piece(int x, int y);
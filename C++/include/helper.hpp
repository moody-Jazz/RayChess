#pragma once
#include "raylib.h"
#include <unordered_map>
#include <string>
#include "../include/gamesound.hpp"

// this header file contains all the necessary stuff related to ui managment like piece textures
extern int tileSize;
extern Color light, dark, bg;
extern int topPadding, leftPadding;

enum{white, black, both};

enum{P,N,B,R,Q,K,p,n,b,r,q,k};

enum{ wk, wq, bk, bq};

enum{ left, top, right, bottom};

enum{
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1, no_sq
};
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

extern PieceUI pieceOnBoard[32];
extern PieceUI PieceCaptured[30];

extern int totalPiece;
extern int totalCaptured;

void InitializePiece(PieceUI *piece, char type, Texture2D texture, int row, int col);

void InitializePieces(Texture2D texture[]);

void deletePiece(PieceUI *piece);

PieceUI* isThereA_PieceUI(int x, int y);
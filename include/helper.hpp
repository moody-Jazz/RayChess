#pragma once

#include <string>
#include <unordered_map>
#include "gamesound.hpp"

typedef struct PieceUI                      // Represents a Visual piece with its corrosponding texture and coo3rdinate on board
{
    char type;
    Texture2D texture;
    size_t row;
    size_t col;
} PieceUI;

namespace Globals 
{
    extern size_t tileSize;                 // Dimension of single tile
    extern size_t totalPiece;               // Stores the number of total Pieces which are still on the board
    extern size_t totalCaptured;            // Stores the number of total pieces that have been captured
    extern size_t topPadding;               // Stores the height of top File strip
    extern size_t leftPadding;              // Stores the width of left Rank strip

    extern Color light;                     // Stores the color of light tiles
    extern Color dark;                      // Stores the color of dark tile
    extern Color bg;                        // Stores the BG color 

    extern const std::string asciiPieces;   // "PNBRQKpnbrqk" stores this string 
    extern const std::unordered_map<char, int> charPieces; // Map piece character to its corrosponding enum 

    extern GameSound sound;                 // Stores all the sound like capture, move etc        

    extern PieceUI pieceOnBoard[32];        // Stores information about the pieces on Board
    extern PieceUI pieceCaptured[30];       // Stores information about the pieces that have been captured
}

// Enums to organize constants
enum { white, black, both };
enum { P, N, B, R, Q, K, p, n, b, r, q, k };// used to iterate to all the pieces and acess a piece like 'P' for white pawn
enum { kingside, queenside };
enum { left, top, right, bottom };
enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1, no_sq
};

// Function declarations
void InitializePiece(PieceUI* piece, char type, Texture2D texture, int row, int col);
void InitializePieces(Texture2D texture[]);
void deletePiece(PieceUI* piece);
PieceUI* isThereAPieceTexture(size_t x, size_t y);
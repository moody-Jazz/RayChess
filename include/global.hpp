#pragma once

#include <string>
#include <cstdint>
#include <fstream>
#include <unordered_map>
#include "gamesound.hpp"

namespace Globals 
{   extern bool player;                  // Stores the color which player has chosen
    extern bool engineToggleOn;          // stores whether the player want to play against engine or not
    extern size_t depth;                 // stores the depth of search tree
    extern size_t windowWidth;           // Stores the width of the game window
    extern size_t windowHeight;          // Stores the height of the game window
    extern size_t tileSize;              // Size of single tile, tiles is square so width and hight is same
    extern float capturedSize;           // Stores the size of captured piece texture
    extern size_t totalPiece;            // Stores the number of total Pieces which are still on the board
    extern size_t topPadding;            // Stores the height of top File strip
    extern size_t leftPadding;           // Stores the width of left Rank strip
    extern size_t btnHeight;             
    extern size_t btnWidth;              
    extern size_t boardSize;             // Stores the size of chessboard
    extern Image icon;                   // icons for the application

    extern std::string FENString;        // Stores the state of the board in FEN notation
    extern std::ofstream outFile;        // TXT file to store state of board after each turn
    extern std::string testPosition[5];  // Array to store positions for testing purpose

    extern GameSound sound;              // Stores all the sound like capture, castle etc        

    extern std::string pieceDir[12];     // Stores the path of all the pieces in image folder
    extern Texture2D pieceTextures[12];  // Stores all the texture in order of "PNBRQKpnbrqk";
    extern Texture2D captureTextures[12];// Stores the textures for captured pieces

    extern const uint64_t rankZero;
    extern const uint64_t rankEight;
    extern const uint64_t notAFile;      // Stores a binary representation where A file is set to 0
    extern const uint64_t notHFile;      // Stores a binary representation where A file is set to 0
    extern const uint64_t notGHFile;     // Stores a binary representation where A file is set to 0
    extern const uint64_t notABFile;     // Stores a binary representation where A file is set to 0

    extern size_t kingsInitPos[2];       // Stores initial position of black and white king
    extern size_t queenInitPos[2];       // Stores initial position of black and white queen
    extern size_t rookInitPos[2][2];     // Stores initial position of all the rooks

    extern size_t castleRookTargetTiles[2][2];  // Stores the tiles on which rook moves while casteling
    extern size_t castleKingTargetTile[2][2];   // stores the tiles on which king moves while casteling

    extern uint64_t castleBitmasks[2][3];       // stores the absolute bitmask used to check castle availability    
    extern uint64_t occupancyBitmask[2];        // Stores the bitmask where tiles between king and rook set to 1

    void setUIparams();                         // initialises all the paramter related to the scale of all ui elements
}

namespace Colors
{
    extern Color lightTile;              // Color of light tiles
    extern Color darkTile;               // Color of dark tiles
    extern Color boardOutline;           // Color of the border strip of the board
    extern Color tileHighlight;          // Color used to highlight the tiles on which we can move
    extern Color danger;                 // Color used to highlight the king's tiles when in check
    extern Color background;             // Backgound Color of the game

    extern Color btnBase;                // Button Color
    extern Color btnBorder;              // Button border color
    extern Color labelColor;             // Color of text inside the button
    extern Color btnHighlight;           // Color when btn is cliked to hightlight it
}

// Enums to organize constants
enum { white, black, both };
enum { regular, capture, castle, check, checkmate, stalemate };
enum { P, N, B, R, Q, K, p, n, b, r, q, k };  // used to iterate to all the pieces and acess a piece like 'P' for white pawn
enum { kingside, queenside };
enum { left, top, right, bottom };
enum { knightProm = 4096, bishopProm = 8192, rookProm = 12288, queenProm = 16384 };
enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1, noSq
};

extern const std::string asciiPieces;                               // "PNBRQKpnbrqk" stores this string 
extern const std::unordered_map<char, int> charPieces;              // Map piece character to its corrosponding enum 
extern const std::string coordinate[64];
extern const std::unordered_map<std::string, int> coordsToAbsolute;     // map the coordinates to thier absolute value

// Function declarations
void loadPieceTextures();               // loads the texture of pieces into the VRAM
void unloadPieceTextures();              // function to unload textures from VRAM
bool getType(char type);                // Returns the color type of the piece
bool flipType(char type);               // flips the color type of piece
char flipCase(char ch);                 // flips the piece to its counter type
void playSound(size_t moveType);        // plays sound based on the movetype
uint16_t moveEncoder(uint16_t srcTile, uint16_t destTile, uint16_t promo);  // encodes move into 16bit positive integer
void moveDecoder(uint16_t& srcTile, uint16_t& destTile, uint16_t& promo, uint16_t move); // extract src, destination and promotion from encoded move
std::string getAlgebricNotation(uint16_t& move, bool side); // returns the algebric notation of the encoeded move
void writeInFile(std::string FEN);      // this function writes teh fen value of current position in a text file
void centerWindow();
void resizeWindow();
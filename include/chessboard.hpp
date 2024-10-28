#pragma once

#include "helper.hpp"
#include "bitboard.hpp"

static const std::string coordinate[] = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
};

/*  
    notAFile            notHFile            notGHFile           notABFile 
    0 1 1 1 1 1 1 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0     0 0 1 1 1 1 1 1
    0 1 1 1 1 1 1 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0     0 0 1 1 1 1 1 1
    0 1 1 1 1 1 1 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0     0 0 1 1 1 1 1 1
    0 1 1 1 1 1 1 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0     0 0 1 1 1 1 1 1
    0 1 1 1 1 1 1 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0     0 0 1 1 1 1 1 1
    0 1 1 1 1 1 1 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0     0 0 1 1 1 1 1 1
    0 1 1 1 1 1 1 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0     0 0 1 1 1 1 1 1
    0 1 1 1 1 1 1 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0     0 0 1 1 1 1 1 1
    a b c d e f g h     a b c d e f g h     a b c d e f g h     a b c d e f g h  
*/
// Below are the Decimal representation of above binary
static const uint64_t notAFile = 18374403900871474942ULL;   
static const uint64_t notHFile = 9187201950435737471ULL;
static const uint64_t notGHFile = 4557430888798830399ULL;
static const uint64_t notABFile = 18229723555195321596ULL;

class Board
{
public:
    size_t turn;                    // Stores which player to move
    BitBoard legalMoves;            // Stores what legal moves are available on board for a piece 
    size_t enPassant;               // Stores the square on which enPassant is available
    bool castle[2][2];              // Stores Booleans indicating whether each side castle for each player is available
    uint64_t castleBitmasks[2][3];  // stores the absolute bitmask used to check castle availability
    uint64_t bitboards[3];          // Stores Bitboard for white black and both
    float padding[4];               // Stores the padding on each direction to determine the dimension of File and Rank strips

    Board();

    // Utility Functions
    void drawTiles() const;                     // draw the checkers pattern
    void drawStrips() const;                    // draw the rank and file strips
    void drawUpdatedBoard() const;              // draw all the piece texture after a move has been exectued
    void print() const;                         // Prints the current position as ASCII characters in matrix form
    void highlightTiles(BitBoard tiles) const;  // Highlight given tiles

    // Mutator Functions
    void flipTurn();                                // flip turns
    void updateMatrixBoard();                       // updates the matrixBoard after every turn
    void updateFENViamatrixBoard();                 // Update the FEN string using the matrix board
    void syncBitboards(BitBoard *pieceBitboards);   // Updates the 3 member bitboards using piece bitboards

    // Interacting Functions (interacts with other classes)
    void makeMove(PieceUI *currPiece, int releasedOnTileRow, int releasedOnTileCol, BitBoard *pieceBitboards, size_t *kingPosition);

private:
    size_t emptyTurns_;              // Stores the number of consecutive turns without captures or pawn moves
    size_t totalTurns_;              // Stores total turns that have been played
    char matrixBoard_[8][8];         // Stores the current position in matrix form
    std::string FENString_;          // Stores the current position as FEN string
};
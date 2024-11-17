#pragma once

#include "helper.hpp"
#include "piece.hpp"
#include <memory>

class Board
{
public:
    Board();

    size_t turn;                    // Stores which player to move
    BitBoard legalMoves;            // Stores what legal moves are available on board for a piece 
    float padding[4];               // Stores the padding on each direction to determine the dimension of File and Rank strips
    Piece piece;

    // Utility Functions
    void drawTiles() const;                     // draw the checkers pattern
    void drawStrips() const;                    // draw the rank and file strips
    void drawBoardAndPieces() const;              // draw all the piece texture after a move has been exectued
    void print() const;                         // Prints the current position as ASCII characters in matrix form
    void highlightTiles(BitBoard tiles) const;  // Highlight given tiles

    // Mutator Functions
    void flipTurn();                                // flip turns
    void updateMatrixBoard();                       // updates the matrixBoard after every turn
    void updateFENViamatrixBoard();                 // Update the FEN string using the matrix board

    // Interacting Functions (interacts with other classes)
    size_t makeMove(std::string move);  // executes a move on the boards and returns what type of move it is

private:
    size_t emptyTurns_;              // Stores the number of consecutive turns without captures or pawn moves
    size_t totalTurns_;              // Stores total turns that have been played
    char matrixBoard_[8][8];         // Stores the current position in matrix form
};
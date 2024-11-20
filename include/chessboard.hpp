#pragma once

#include "global.hpp"
#include "piece.hpp"
#include <memory>

class Board
{
public:
    Board();

    size_t turn;                                // Stores which player to move
    BitBoard legalMoves;                        // Stores what legal moves are available on board for a piece 
    float padding[4];                           // Stores the padding on each direction to determine the dimension of File and Rank strips
    Piece piece;                                // Objects to store positions and states of pieces on board
    std::string capturedPieceString;            // Stores a string indicating all the captured pieces

    // Utility Functions
    void drawTiles() const;                     // Draw the checkers pattern
    void drawStrips() const;                    // Draw the rank and file strips
    void drawBoardAndPieces() const;            // Draw all the piece texture after a move has been exectued
    void drawCapturedPieces() const;            // Draws all the captured pieces
    void print() const;                         // Prints the current position as ASCII characters in matrix form
    void highlightTiles(BitBoard tiles) const;  // Highlight given tiles

    // Mutator Functions
    void setupInitialBoardState();               // Setup all the flags and varaibles to their initial state also used to reset the board
    void flipTurn();                             // Flips turn after every move
    void updateMatrixBoard();                    // Updates the matrixBoard after every turn
    void updateFENViamatrixBoard();              // Update the FEN string using the matrix board
    size_t findTotalLegalMoves(bool side);       // Finds the total number of legal moves a side has
    size_t getGameEndState(bool side);           // Determines and returns whether the game ended in checkmate or stalemate

    size_t makeMove(std::string move);           // Executes a move on the boards and returns what type of move it is

private:
    size_t emptyTurns_;                          // Stores the number of consecutive turns without captures or pawn moves
    size_t totalTurns_;                          // Stores total turns that have been played
    char matrixBoard_[8][8];                     // Stores the current position in matrix form
};
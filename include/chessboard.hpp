#pragma once

#include "global.hpp"
#include "piece.hpp"
#include <memory>

class Board
{
    public:
    Board();
    
    Board(const Board& obj);
    size_t turn;                                // Stores which player to move
    BitBoard legalMoves;                        // Stores what legal moves are available on board for a piece 
    float padding[4];                           // Stores the padding on each direction to determine the dimension of File and Rank strips
    Piece piece;                                // Objects to store positions and states of pieces on board
    std::string capturedPieceString;            // Stores a string indicating all the captured pieces
    size_t emptyTurns;                          // Stores the number of consecutive turns without captures or pawn moves
    size_t totalTurns;                          // Stores total turns that have been played
    char matrixBoard[8][8];                     // Stores the current position in matrix form

    // Utility Functions
    void drawTiles() const;                     // Draw the checkers pattern
    void drawStrips() const;                    // Draw the rank and file strips
    void drawBoardAndPieces() const;            // Draw all the piece texture after a move has been exectued
    void drawCapturedPieces() const;            // Draws all the captured pieces
    void print() const;                         // Prints the current position as ASCII characters in matrix form
    void highlightTiles(BitBoard tiles) const;  // Highlight given tiles
    void getMoveList(uint16_t* res, uint16_t& curr, bool side) const; // Return a list of all the legal moves a side have
    uint16_t printMoveList(bool side) const;       // prints Move list of the pieces of a side

    // Mutator Functions
    void setUIparams();                          // set value of the padding 
    void setupInitialBoardState();               // Setup all the flags and varaibles to their initial state also used to reset the board
    void flipTurn();                             // Flips turn after every move
    void copyPosition(std::string& FEN);         // copies a position using FEN string
    void updateMatrixBoard();                    // Updates the matrixBoard after every turn
    void updateFENViamatrixBoard();              // Update the FEN string using the matrix board
    size_t findTotalLegalMoves(bool side);       // Finds the total number of legal moves a side has
    size_t getGameEndState(bool side);           // Determines and returns whether the game ended in checkmate or stalemate

    size_t makeMove(uint16_t move);              // Executes a move on the boards and returns what type of move it is

};
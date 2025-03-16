#include "chessboard.hpp"
#include <chrono>

class Engine
{
    public:
    int evaluate(BitBoard* piece);      // function to evaluate a position based on material value and positional value
    uint64_t perft(Board& board, int depth, bool turn); // Function to count all the terminal nodes at a depth
    void perfomenceCheck(Board& board, uint16_t depth); // Function to calculate the time to visit all the nodes/position in search tree
    std::pair<int, uint16_t> minimax(Board& board, size_t depth, int alpha, int beta, bool turn, uint16_t initialMove, uint64_t& total); // finds the best move
};
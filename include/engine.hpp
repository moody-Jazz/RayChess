#include "chessboard.hpp"

class Engine{
    public:
    int evaluate(BitBoard* piece);
    uint64_t perft(Board& board, int depth, bool turn);
    std::pair<int, uint16_t> minimax(Board& board, size_t depth, int alpha, int beta, bool turn, uint16_t initialMove, uint64_t& total);
};
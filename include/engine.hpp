#include "chessboard.hpp"

class Engine{
    public:
    int evaluate(BitBoard* piece);
    uint64_t perft(Board& board, int depth, bool turn);
    std::pair<int, std::string> minimax(Board& board, size_t depth, int alpha, int beta, bool turn, std::string initialMove, uint64_t& total);
};
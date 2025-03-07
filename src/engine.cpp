#include "engine.hpp"
#include <iostream>
#include <vector>

/*
♙ =   100   = ♙
♘ =   300   = ♙ * 3
♗ =   350   = ♙ * 3 + ♙ * 0.5
♖ =   500   = ♙ * 5
♕ =   1000  = ♙ * 10
♔ =   10000 = ♙ * 100
*/

const int material_score[12] = {
    100,      // white pawn score
    300,      // white knight scrore
    350,      // white bishop score
    500,      // white rook score
    1000,      // white queen score
    10000,      // white king score
    -100,      // black pawn score
    -300,      // black knight scrore
    -350,      // black bishop score
    -500,      // black rook score
    -1000,      // black queen score
    -10000,      // black king score
};

// pawn positional score
const int pawn_score[64] = 
{
    90,  90,  90,  90,  90,  90,  90,  90,
    30,  30,  30,  40,  40,  30,  30,  30,
    20,  20,  20,  30,  30,  30,  20,  20,
    10,  10,  10,  20,  20,  10,  10,  10,
    5,   5,  10,  20,  20,   5,   5,   5,
    0,   0,   0,   5,   5,   0,   0,   0,
    0,   0,   0, -10, -10,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0
};

// knight positional score
const int knight_score[64] = 
{
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,  10,  10,   0,   0,  -5,
    -5,   5,  20,  20,  20,  20,   5,  -5,
    -5,  10,  20,  30,  30,  20,  10,  -5,
    -5,  10,  20,  30,  30,  20,  10,  -5,
    -5,   5,  20,  10,  10,  20,   5,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5, -10,   0,   0,   0,   0, -10,  -5
};

// bishop positional score
const int bishop_score[64] = 
{
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,  10,  10,   0,   0,   0,
    0,   0,  10,  20,  20,  10,   0,   0,
    0,   0,  10,  20,  20,  10,   0,   0,
    0,  10,   0,   0,   0,   0,  10,   0,
    0,  30,   0,   0,   0,   0,  30,   0,
    0,   0, -10,   0,   0, -10,   0,   0

};

// rook positional score
const int rook_score[64] =
{
    50,  50,  50,  50,  50,  50,  50,  50,
    50,  50,  50,  50,  50,  50,  50,  50,
    0,   0,  10,  20,  20,  10,   0,   0,
    0,   0,  10,  20,  20,  10,   0,   0,
    0,   0,  10,  20,  20,  10,   0,   0,
    0,   0,  10,  20,  20,  10,   0,   0,
    0,   0,  10,  20,  20,  10,   0,   0,
    0,   0,   0,  20,  20,   0,   0,   0

};

// king positional score
const int king_score[64] = 
{
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   5,   5,   5,   5,   0,   0,
    0,   5,   5,  10,  10,   5,   5,   0,
    0,   5,  10,  20,  20,  10,   5,   0,
    0,   5,  10,  20,  20,  10,   5,   0,
    0,   0,   5,  10,  10,   5,   0,   0,
    0,   5,   5,  -5,  -5,   0,   5,   0,
    0,   0,   5,   0, -15,   0,  10,   0
};

// mirror positional score tables for opposite side
const int mirror_score[128] =
{
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8
};

int Engine::evaluate(BitBoard* piece){
    int score{};
    for(size_t i = P; i<=k; i++){
        BitBoard piececpy;
        piececpy.setVal(piece[i].getVal());

        score += (piececpy.getSetBitCount() * material_score[i]);

        std::vector<size_t> piece_position = piececpy.getSetBitIndices();

        for(auto& square: piece_position){
            square = 63 - square;
            switch (i)
            {
                // evaluate white pieces
                case P: score += pawn_score[square]; break;
                case N: score += knight_score[square]; break;
                case B: score += bishop_score[square]; break;
                case R: score += rook_score[square]; break;
                case K: score += king_score[square]; break;

                // evaluate black pieces
                case p: score -= pawn_score[mirror_score[square]]; break;
                case n: score -= knight_score[mirror_score[square]]; break;
                case b: score -= bishop_score[mirror_score[square]]; break;
                case r: score -= rook_score[mirror_score[square]]; break;
                case k: score -= king_score[mirror_score[square]]; break;
            }
        }
    }
    return score;
}

uint64_t Engine::perft(Board& board, int depth, bool turn) {
    if (depth == 0) return 1;
    uint64_t nodes = 0;
    std::vector<std::string> moveList = board.getMoveList(turn);
    for (auto& move : moveList) {
        Board boardCpy(board);
        boardCpy.makeMove(move);
        nodes += perft(boardCpy, depth - 1, !turn);
    }
    return nodes;
}


std::pair<int, std::string> Engine::minimax(Board& board, size_t depth,int alpha, int beta, bool turn, std::string initialMove, uint64_t& total)
{
    total++;
    if(depth == 0) return {evaluate(board.piece.pieceBitboards), initialMove};
    
    std::pair<int, std::string> maxEval;
    maxEval.first = turn ? INT_MAX: INT_MIN;
    
    std::vector<std::string> moveList = board.getMoveList(turn);

    if(board.emptyTurns_ >= 50) return {0, initialMove};
    
    if(moveList.size() == 0){
        if (board.piece.isKingSafe(turn)) return {0, initialMove};
        return turn? std::make_pair(1000000+depth, initialMove) : std::make_pair(-1000000-depth, initialMove);
    }
    for(auto& move: moveList)
    {
        if(depth == Globals::depth) initialMove = move;
        Board boardCpy(board);
        boardCpy.makeMove(move);
        std::pair<int, std::string> eval = minimax(boardCpy, depth-1, alpha, beta, !turn, initialMove, total);

        if(!turn)
        {
            if(maxEval.first < eval.first) maxEval = eval;
            alpha = std::max(alpha, maxEval.first);
            if(beta <= alpha) break;
        }
        else
        {
            if(maxEval.first > eval.first) maxEval = eval;
            beta = std::min(beta, maxEval.first);
            if(beta <= alpha) break;
        }
    }
    return maxEval;
}
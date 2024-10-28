#include "piece.hpp"
#include <iostream>

Piece::Piece(){
    /* 
    all the pieces will be initialised with a decimal value which in binary is similar to initial chess pieces position

    White pawn bitboard                 black pawn bitboard                 black rook bitboard

    8  0 0 0 0 0 0 0 0                  8  0 0 0 0 0 0 0 0                  8  1 0 0 0 0 0 0 1            
    7  0 0 0 0 0 0 0 0                  7  1 1 1 1 1 1 1 1                  7  0 0 0 0 0 0 0 0 
    6  0 0 0 0 0 0 0 0                  6  0 0 0 0 0 0 0 0                  6  0 0 0 0 0 0 0 0 
    5  0 0 0 0 0 0 0 0                  5  0 0 0 0 0 0 0 0                  5  0 0 0 0 0 0 0 0 
    4  0 0 0 0 0 0 0 0                  4  0 0 0 0 0 0 0 0                  4  0 0 0 0 0 0 0 0 
    3  0 0 0 0 0 0 0 0                  3  0 0 0 0 0 0 0 0                  3  0 0 0 0 0 0 0 0
    2  1 1 1 1 1 1 1 1                  2  0 0 0 0 0 0 0 0                  2  0 0 0 0 0 0 0 0
    1  0 0 0 0 0 0 0 0                  1  0 0 0 0 0 0 0 0                  1  0 0 0 0 0 0 0 0

       a b c d e f g h                     a b c d e f g h                     a b c d e f g h
    */

    pieceBitboards[P].setVal(65280ULL);
    pieceBitboards[N].setVal(66ULL);
    pieceBitboards[B].setVal(36ULL);
    pieceBitboards[R].setVal(129ULL);
    pieceBitboards[Q].setVal(16ULL);
    pieceBitboards[K].setVal(8ULL);
    pieceBitboards[p].setVal(71776119061217280ULL);
    pieceBitboards[n].setVal(4755801206503243776ULL);
    pieceBitboards[b].setVal(2594073385365405696ULL);
    pieceBitboards[r].setVal(9295429630892703744ULL);
    pieceBitboards[q].setVal(1152921504606846976ULL);
    pieceBitboards[k].setVal(576460752303423488ULL);

    check[white] = check[black] = false;

    kingPosition[white] = 3;
    kingPosition[black] = 59;

    unsafeTiles[white] = unsafeTiles[black] = 0ULL;
}

/*  Below are all the functions which are used to generate possible moves for every type of piece
    i have used some algorithms which do bitmanipulation to geenrate some numbers which in binary representation 
    have all those ith bits which represnt the tiles that can be reached by the current piece are set to 1 for example
    u64 knightAttackBitmask(int square) function takes the position of the knight and returns a number
    (attacks) which have the attacking squares set as 1 bit
    
    we can do similar to slider pieces while doing bit operation with blocker pieces(which may contian 
    friendly and enemy bitboard) to find correct possible moves
*/
uint64_t Piece::pawnAttackBitmaskInit(size_t side, size_t square) const
{
    uint64_t attacks(0ULL);
    uint64_t bitboard(0ULL);
    bitboard |= (1ULL << square);
    //black pawns
    if(side)
    {
        if((bitboard >> 7) & notAFile) attacks |= (bitboard >> 7);
        if((bitboard >> 9) & notHFile) attacks |= (bitboard >> 9);
    }
    //white pawns
    else
    {
        if((bitboard << 7) & notHFile) attacks |= (bitboard << 7);
        if((bitboard << 9) & notAFile) attacks |= (bitboard << 9);
    }
    return attacks;
}
uint64_t Piece::pawnPushBitmaskInit(size_t side, size_t square) const
{
    uint64_t attacks(0ULL);
    uint64_t bitboard(0ULL);
    bitboard |= (1ULL << square);
    //black pawns
    if(side)
    {
        // if pawn is on initial square then it can move two tiles ahead
        if(square >=48 && square <= 55)
        {
            attacks |= (bitboard >> 8);
            attacks |= (bitboard >> 16); 
        }
        else attacks |= (bitboard >> 8);
    }
    //white pawns
    else
    {
        if(square >=8 && square <= 15)
        {
            attacks |= (bitboard << 8);
            attacks |= (bitboard << 16); 
        }
        else attacks |= (bitboard << 8);
        
    }
    return attacks;
}
uint64_t Piece::knightAttackBitmaskInit(size_t square) const
{
    uint64_t attacks(0ULL);
    uint64_t bitboard(0ULL);
    bitboard |= (1ULL << square);
    //generate knight attacks
    if((bitboard >> 17) & notHFile) attacks |= (bitboard >> 17);
    if((bitboard >> 15) & notAFile) attacks |= (bitboard >> 15);
    if((bitboard >> 10) & notGHFile) attacks |= (bitboard >> 10);
    if((bitboard >> 6) & notABFile) attacks |= (bitboard >> 6); 
    if((bitboard << 17) & notAFile) attacks |= (bitboard << 17);
    if((bitboard << 15) & notHFile) attacks |= (bitboard << 15);
    if((bitboard << 10) & notABFile) attacks |= (bitboard << 10);
    if((bitboard << 6) & notGHFile) attacks |= (bitboard << 6); 
    return attacks;
}
uint64_t Piece::kingAttackBitmaskInit(size_t square) const
{
    uint64_t attacks(0ULL);
    uint64_t bitboard(0ULL); 
    bitboard |= (1ULL << square);

    if(bitboard >> 8) attacks |= (bitboard >> 8);
    if((bitboard >> 9) & notHFile) attacks |= (bitboard >> 9);
    if((bitboard >> 7) & notAFile) attacks |= (bitboard >> 7);
    if((bitboard >> 1) & notHFile)attacks |= (bitboard >> 1);  

    if(bitboard << 8) attacks |= (bitboard << 8);
    if((bitboard << 9) & notAFile) attacks |= (bitboard << 9);
    if((bitboard << 7) & notHFile) attacks |= (bitboard << 7);
    if((bitboard << 1) & notAFile)attacks |= (bitboard << 1);  
    
    return attacks;
}
uint64_t Piece::getBishopAttacks(size_t square, uint64_t block) const
{
    uint64_t attacks(0ULL);
    int r, f;
    int tr = square/8;
    int tf = square % 8;

    for(r = tr +1, f = tf+1; r<=7 && f<=7; r++, f++)
    {
        attacks |= (1ULL << (r*8+f));
        if(block & (1ULL << (r*8+f))) break;
    }
    for(r = tr -1, f = tf+1; r>=0 && f<=7; r--, f++)
    {
        attacks |= (1ULL << (r*8+f));
        if(block & (1ULL << (r*8+f))) break;
    }
    for(r = tr +1, f = tf-1; r<=7 && f>=0; r++, f--)
    {
        attacks |= (1ULL << (r*8+f));
        if(block & (1ULL << (r*8+f))) break;
    }
    for(r = tr -1, f = tf-1; r>=0 && f>=0; r--, f--)
    {
        attacks |= (1ULL << (r*8+f));
        if(block & (1ULL << (r*8+f))) break;
    } 
    return attacks;
}
uint64_t Piece::getRookAttacks(size_t square, uint64_t block) const
{
    uint64_t attacks(0ULL);
    int r,f;
    int tr = square / 8;
    int tf = square % 8;

    for(r = tr+1; r<= 7; r++) 
    {
        attacks |= (1ULL << (r*8 +tf));
        if((1ULL << (r*8 +tf)) & block) break;
    }
    for(r = tr-1; r>= 0; r--) 
    {
        attacks |= (1ULL << (r*8 +tf));
        if((1ULL << (r*8 +tf)) & block) break;
    }
    for(f = tf + 1; f<=7; f++) 
    {
        attacks |= (1ULL << (tr *8 +f));
        if((1ULL << (tr *8 +f)) & block) break;
    }
    for(f = tf - 1; f>=0; f--) 
    {
        attacks |= (1ULL << (tr *8 +f));
        if((1ULL << (tr *8 +f)) & block) break;
    }
    return attacks;
}
uint64_t Piece::getQueenAttacks(size_t square, uint64_t block) const
{
    uint64_t attacks(0ULL);
    attacks |= getBishopAttacks(square, block);
    attacks |= getRookAttacks(square, block);
    return attacks;
}

void Piece::updatePieceBitboard(char type, size_t src, size_t dest)
{
    pieceBitboards[Globals::charPieces.at(type)].setBit(dest);
    pieceBitboards[Globals::charPieces.at(type)].popBit(src);
}

bool Piece::isKingSafe(bool turn) const
{
    return !((1ULL << kingPosition[turn]) & unsafeTiles[turn]);
}

char Piece::isTherePiece(size_t tile) const
{
    for (int i = P; i <= k; i++)
        if((1ULL<<tile) & pieceBitboards[i].getVal()) return Globals::asciiPieces[i];
    
    return '0';
}
uint64_t Piece::getPseudoLegalMoves(Board board, char type, size_t square) const
{
    char piece = toupper(type);
    size_t turn = (type < 'Z')?0:1;
    uint64_t res = 0ULL;
    
    switch (piece)
    {
        case 'P': 
        {
           // find legal attack moves
            uint64_t pawn_attack = pawnAttackBitmask[turn][square] & board.bitboards[!turn];
            // if there is an enpassant available a pawn can catpure it add it to pawn attack
            if(board.enPassant != no_sq)
                pawn_attack |= (pawnAttackBitmask[turn][square] & (1ULL << (63 - board.enPassant)));
            
            // find legal push moves
            uint64_t pawn_push = pawnPushBitmask[turn][square] & ~(board.bitboards[both]);

            /*
            check if there is a piece directly infront of pawn if so then return zero legal push moves
            this if condition is necessary because on thier initial squares pawn can move two tiles 
            if the condition below doen't exist then the pawn will have one legal move which is 
            behind the blocker piece even when a piece is directly in front of it 

            legal moves if the below if() condition is not there

                5   - - - - - - - -
                4   1 - R - 1 - - 1
                3   1 - 1 - B - - r
                2   P - P - P - - P
                1   - - - - - - - -
            */ 
            if((!turn && 1ULL<<(square+8) & board.bitboards[both]) || (turn && 1ULL<<(square-8) & board.bitboards[both]))
                pawn_push = 0ULL; 
            res = pawn_attack | pawn_push;
            break;
        }
        case 'N': 
        {
            res = knightAttackBitmask[square] & ~(board.bitboards[turn]);
            break;
        }
        case 'K': 
        {
            res = kingAttackBitmask[square] & ~(board.bitboards[turn]);
            bool kingSafety = isKingSafe(turn);
            /* 
            below are the bitmanipulation operations to find whether casteling is available or not
            first do operations with unsafe tiles and blocker pieces if there are no blocker pieces our bitmask won't change
            now we can use kingSide bitmask and queenSide bitmask with the resultent castle bitmask to find valid casteling

            decimal vlaues used to find white valid casteling

            54ULL = 0 0 0 0 0 0 0 0     6ULL =  0 0 0 0 0 0 0 0    48ULL = 0 0 0 0 0 0 0 0 
                    0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0            0 0 0 0 0 0 0 0
                    0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0            0 0 0 0 0 0 0 0
                    0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0            0 0 0 0 0 0 0 0
                    0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0            0 0 0 0 0 0 0 0
                    0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0            0 0 0 0 0 0 0 0
                    0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0            0 0 0 0 0 0 0 0
                    0 0 1 1 0 1 1 0             0 0 0 0 0 1 1 0            0 0 1 1 0 0 0 0
                    castleBitmask             kingside castleBitmask     queenside castlebitmask
                
            similarly different numbers have been used to do the same for black
            */
            uint64_t castleBitmasks = board.castleBitmasks[turn][both];
            uint64_t bitmaskKingside = board.castleBitmasks[turn][kingside];
            uint64_t bitmaskQueenside = board.castleBitmasks[turn][queenside];

            castleBitmasks &= ~unsafeTiles[turn] & ~board.bitboards[both];

            // check if kingside castling is available
            bool castleAvailable = (castleBitmasks & bitmaskKingside) == bitmaskKingside;
            castleAvailable = castleAvailable && kingSafety && board.castle[turn][kingside];
            res = res | (bitmaskKingside * castleAvailable);

            // check if queenside castling is avialble
            castleAvailable = (castleBitmasks & bitmaskQueenside) == bitmaskQueenside;
            castleAvailable = castleAvailable && kingSafety && board.castle[turn][kingside];
            res = res | (bitmaskQueenside * castleAvailable);
            
            break;
        }
        case 'R': 
        {
            res = getRookAttacks(square, board.bitboards[both]) & ~(board.bitboards[turn]);
            break;
        }
        case 'B': 
        {
            res = getBishopAttacks(square, board.bitboards[both]) & ~(board.bitboards[turn]);
            break;
        }
        case 'Q': 
        {
            res = getQueenAttacks(square, board.bitboards[both]) & ~(board.bitboards[turn]);
            break;
        }
        default:
        {
            break;
        }
    }
    return res;
}

void Piece::updateUnsafeTiles(Board board)
{
    unsafeTiles[white] = unsafeTiles[black] = 0ULL;
    
    // find all the possible moves of white and add it to unsafeTiles for black and vice versa
    for(int i = P; i<=k; i++)
    {
        BitBoard pieceBitboards(this->pieceBitboards[i].getVal());
        while (pieceBitboards.getVal())
        {
            bool turn = (i<=K)? 1: 0;
            size_t sourceTile = pieceBitboards.getLSBIndex();
            unsafeTiles[turn] |= getPseudoLegalMoves(board, Globals::asciiPieces[i], sourceTile);
            pieceBitboards.popBit(sourceTile);
        }
    }
}

uint64_t Piece::getLegalMoves(Board board, char type, size_t source)
{
    size_t turn = (type < 'Z')?0:1;
    size_t piece = Globals::charPieces.at(type);

    // Create a temporary copy of bitboards, unsafeTile, and king position
    uint64_t tempUnsafe[2];
    tempUnsafe[white] = unsafeTiles[white];
    tempUnsafe[black] = unsafeTiles[black];

    uint64_t bitboards[3];
    bitboards[white] = board.bitboards[white];
    bitboards[black] = board.bitboards[black];
    bitboards[both] = board.bitboards[both];

    BitBoard piece_bitboard(this->pieceBitboards[piece].getVal());

    size_t tempKingPosition[2];
    tempKingPosition[white] = kingPosition[white];
    tempKingPosition[black] = kingPosition[black];

    // find the possible moves
    BitBoard possibleMoves(getPseudoLegalMoves(board, type, source));
    uint64_t validMoves = 0ULL;

    while(possibleMoves.getVal())
    {
        size_t destTile = possibleMoves.getLSBIndex();
        char capturedPieceType = isTherePiece(destTile);
        uint64_t capturedBitboard = 0ULL;

        if(capturedPieceType != '0')
        { // if the possible move is capture of enemy piece
            capturedBitboard = pieceBitboards[Globals::charPieces.at(capturedPieceType)].getVal();
            updatePieceBitboard(capturedPieceType, destTile, destTile);
        }

        // if king is bieng moved then update the king position as well
        if(type == 'k' || type == 'K') kingPosition[turn] = destTile;

        // update all the variables representing the state of board and pieceSet according to move
        updatePieceBitboard(type, source, destTile);
        board.syncBitboards(this->pieceBitboards);
        updateUnsafeTiles(board);

        //if king is safe after executing the current possible move then consider this as a valid move
        if(isKingSafe(turn)) validMoves = validMoves | (1ULL << destTile); 

        // reset everything using all the temporary variables
        if(capturedPieceType != '0') pieceBitboards[Globals::charPieces.at(capturedPieceType)].setVal(capturedBitboard);

        unsafeTiles[white] = tempUnsafe[white];
        unsafeTiles[black] = tempUnsafe[black];

        board.bitboards[white] = bitboards[white];
        board.bitboards[black] = bitboards[black];
        board.bitboards[both] = bitboards[both];

        this->pieceBitboards[piece].setVal(piece_bitboard.getVal());

        kingPosition[white] = tempKingPosition[white];
        kingPosition[black] = tempKingPosition[black];
        possibleMoves.popBit(destTile);
    }
    return validMoves;
}
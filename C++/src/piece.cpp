#include "../include/piece.hpp"
#include "../include/bitboard.hpp"
#include "../include/chessboard.hpp"
#include "../include/helper.hpp"
#include <iostream>

/*
=======================================================================================================================================================================================================
                    Constructor to setup the pieces on thier initial squares, all the decimal numbers represent their piece bitboard when converted into binary
=======================================================================================================================================================================================================
*/
Piece::Piece(){
    /* 
    all the pieces will be initialised with a decimal value which in is similar to initial chess pieces position

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

    piece_set[P].set_val(65280ULL);
    piece_set[N].set_val(66ULL);
    piece_set[B].set_val(36ULL);
    piece_set[R].set_val(129ULL);
    piece_set[Q].set_val(16ULL);
    piece_set[K].set_val(8ULL);
    piece_set[p].set_val(71776119061217280ULL);
    piece_set[n].set_val(4755801206503243776ULL);
    piece_set[b].set_val(2594073385365405696ULL);
    piece_set[r].set_val(9295429630892703744ULL);
    piece_set[q].set_val(1152921504606846976ULL);
    piece_set[k].set_val(576460752303423488ULL);

    check[white] = check[black] = false;

    kingPosition[white] = 3;
    kingPosition[black] = 59;

    unsafe_tiles[white] = unsafe_tiles[black] = 0ULL;
}

/*
=======================================================================================================================================================================================================
                    Below are all the functions which are used to generate possible moves for every type of piece
=======================================================================================================================================================================================================
*/
/*  
    i have used some algorithms which do bitmanipulation to geenrate some numbers which in binary representation 
    have all those ith bits which represnt the tiles that can be reached by the current piece are set to 1 for example
    u64 knight_attack_bitmask(int square)
    this function takes the position of the knight and returns a number (attacks) which have the attacking squares set as 1 bit
    
    we can do similar to slider pieces while doing bit operation with blocker pieces(friendly and enemy bitboard) to find correct moves
*/
uint64 Piece::pawn_attack_bitmask_init(int side, int square){
    
    // result attacks bitboard
    BitBoard attacks(0ULL);

    // piece bitboard
    BitBoard bitboard(0ULL);

    //the current position of piece
    bitboard.set_bit(square);

    //black pawns
    if(side){
        if((bitboard.val >> 7) & not_a_file) attacks.val |= (bitboard.val >> 7);
        if((bitboard.val >> 9) & not_h_file) attacks.val |= (bitboard.val >> 9);
    }
    //white pawns
    else{
        if((bitboard.val << 7) & not_h_file) attacks.val |= (bitboard.val << 7);
        if((bitboard.val << 9) & not_a_file) attacks.val |= (bitboard.val << 9);
    }
    // return attack bitboard
    return attacks.val;
}
uint64 Piece::pawn_push_bitmask_init(int side, int square){
    BitBoard attacks(0ULL);

    // piece bitboard
    BitBoard bitboard(0ULL);

    //the current position of piece
    bitboard.set_bit(square);

    //black pawns
    if(side){
        // if pawn is on initial square then it can move two tiles ahead
        if(square >=48 && square <= 55){
            attacks.val |= (bitboard.val >> 8);
            attacks.val |= (bitboard.val >> 16); 
        }
        else {
            attacks.val |= (bitboard.val >> 8);
        }
    }
    //white pawns
    else{
        if(square >=8 && square <= 15){
            attacks.val |= (bitboard.val << 8);
            attacks.val |= (bitboard.val << 16); 
        }
        else {
            attacks.val |= (bitboard.val << 8);
        }
    }
    // return attack bitboard
    return attacks.val;
}

uint64 Piece::knight_attack_bitmask_init(int square){
    BitBoard attacks(0ULL);

    BitBoard bitboard(0ULL);

    // set piece position
    bitboard.set_bit(square);

    //generate knight attacks
    if((bitboard.val >> 17) & not_h_file) attacks.val |= (bitboard.val >> 17);
    if((bitboard.val >> 15) & not_a_file) attacks.val |= (bitboard.val >> 15);
    if((bitboard.val >> 10) & not_hg_file) attacks.val |= (bitboard.val >> 10);
    if((bitboard.val >> 6) & not_ab_file) attacks.val |= (bitboard.val >> 6);    

    if((bitboard.val << 17) & not_a_file) attacks.val |= (bitboard.val << 17);
    if((bitboard.val << 15) & not_h_file) attacks.val |= (bitboard.val << 15);
    if((bitboard.val << 10) & not_ab_file) attacks.val |= (bitboard.val << 10);
    if((bitboard.val << 6) & not_hg_file) attacks.val |= (bitboard.val << 6); 
    return attacks.val;
}

uint64 Piece::king_attack_bitmask_init(int square){
    BitBoard attacks(0ULL);

    BitBoard bitboard(0ULL); 

    bitboard.set_bit(square);

    if(bitboard.val >> 8) attacks.val |= (bitboard.val >> 8);
    if((bitboard.val >> 9) & not_h_file) attacks.val |= (bitboard.val >> 9);
    if((bitboard.val >> 7) & not_a_file) attacks.val |= (bitboard.val >> 7);
    if((bitboard.val >> 1) & not_h_file)attacks.val |= (bitboard.val >> 1);  

    if(bitboard.val << 8) attacks.val |= (bitboard.val << 8);
    if((bitboard.val << 9) & not_a_file) attacks.val |= (bitboard.val << 9);
    if((bitboard.val << 7) & not_h_file) attacks.val |= (bitboard.val << 7);
    if((bitboard.val << 1) & not_a_file)attacks.val |= (bitboard.val << 1);  
    
    return attacks.val;
}

uint64 Piece::get_bishop_attacks(int square, uint64 block){
    uint64 attacks(0ULL);
    int r, f;
    int tr = square/8;
    int tf = square % 8;

    for(r = tr +1, f = tf+1; r<=7 && f<=7; r++, f++){
        attacks |= (1ULL << (r*8+f));
        if(block & (1ULL << (r*8+f))) break;
    }
    for(r = tr -1, f = tf+1; r>=0 && f<=7; r--, f++){
        attacks |= (1ULL << (r*8+f));
        if(block & (1ULL << (r*8+f))) break;
    }
    for(r = tr +1, f = tf-1; r<=7 && f>=0; r++, f--){
        attacks |= (1ULL << (r*8+f));
        if(block & (1ULL << (r*8+f))) break;
    }
    for(r = tr -1, f = tf-1; r>=0 && f>=0; r--, f--){
        attacks |= (1ULL << (r*8+f));
        if(block & (1ULL << (r*8+f))) break;
    } 

    return attacks;
}
uint64 Piece::get_rook_attacks(int square, uint64 block){
    uint64 attacks(0ULL);

    int r,f;
    int tr = square / 8;
    int tf = square % 8;

    for(r = tr+1; r<= 7; r++) {
        attacks |= (1ULL << (r*8 +tf));
        if((1ULL << (r*8 +tf)) & block) break;

    }
    for(r = tr-1; r>= 0; r--) {
        attacks |= (1ULL << (r*8 +tf));
        if((1ULL << (r*8 +tf)) & block) break;
    }
    for(f = tf + 1; f<=7; f++) {
        attacks |= (1ULL << (tr *8 +f));
        if((1ULL << (tr *8 +f)) & block) break;
    }
    for(f = tf - 1; f>=0; f--) {
        attacks |= (1ULL << (tr *8 +f));
        if((1ULL << (tr *8 +f)) & block) break;
    }
    return attacks;
}
uint64 Piece::get_queen_attacks(int square, uint64 block){
    uint64 attacks(0ULL);
    attacks |= get_bishop_attacks(square, block);
    attacks |= get_rook_attacks(square, block);
    return attacks;
}

void Piece::update_piece_bitboard(char type, int src, int dest){
    this->piece_set[char_pieces.at(type)].set_bit(dest);
    this->piece_set[char_pieces.at(type)].pop_bit(src);
}

/*
=======================================================================================================================================================================================================
                    This function generates all the pseudo legal moves: meaning it doesn't check if a move put the king in danger it still consider them legal
=======================================================================================================================================================================================================
*/
uint64 Piece::get_pseudo_legal_move(Board board, char type, int square){

    char piece = toupper(type);
    int turn = (type < 'Z')?0:1;
    uint64 res = 0ULL;
    
    switch (piece)
    {
        case 'P': {
           // find legal attack moves
            uint64 pawn_attack = pawn_attack_bitmask[turn][square] & board.bitboards[!turn];
            // if there is an enpassant available a pawn can catpure it add it to pawn attack
            if(board.en_passant != -1)
                pawn_attack |= (pawn_attack_bitmask[turn][square] & (1ULL << (63 - board.en_passant)));
            
            // find legal push moves
            uint64 pawn_push = pawn_push_bitmask[turn][square] & ~(board.bitboards[both]);

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
            if((!turn && 1ULL<<(square+8) & board.bitboards[both]) ||
                (turn && 1ULL<<(square-8) & board.bitboards[both])) {
                    pawn_push = 0ULL; 
            }
            res = pawn_attack | pawn_push;
            break;
        }
        case 'N': {
            res = knight_attack_bitmask[square] & ~(board.bitboards[turn]);
            break;
        }
        case 'K': {
            res = king_attack_bitmask[square] & ~(board.bitboards[turn]);
            bool kingSafety = is_king_safe(turn);

            /* 
            below are the bitmanipulation operations to find whether casteling is available or not
            first do the operation with unsafe tiles and blocker pieces if there are none of these pieces our bitmask will not be changed
            now we can use king side castle bitmask and queen side castle bitmask with the resultent castle bitmask to find valid casteling

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

           decimal vlaues used to find black valid casteling

            3891110078048108544ULL       432345564227567616ULL       3458764513820540928ULL
                0 0 1 1 0 1 1 0             0 0 0 0 0 1 1 0             0 0 1 1 0 0 0 0 
                0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0
                0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0
                0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0
                0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0
                0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0
                0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0
                0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0
                castleBitmask             kingside castleBitmask      queenside castlebitmask
            */
            uint64 castle_bitmask = board.castle_bitmasks[turn][both];
            uint64 bitmask_kingside = board.castle_bitmasks[turn][kingside];
            uint64 bitmask_queenside = board.castle_bitmasks[turn][queenside];

            castle_bitmask &= ~unsafe_tiles[turn] & ~board.bitboards[both];

            // check if kingside castling is available
            bool castle_available = (castle_bitmask & bitmask_kingside) == bitmask_kingside;
            castle_available = castle_available && kingSafety && board.castle[turn][kingside];
            res = res | (bitmask_kingside * castle_available);

            // check if queenside castling is avialble
            castle_available = (castle_bitmask & bitmask_queenside) == bitmask_queenside;
            castle_available = castle_available && kingSafety && board.castle[turn][kingside];
            res = res | (bitmask_queenside * castle_available);
            
            break;
        }
        case 'R': {
            res = get_rook_attacks(square, board.bitboards[both]) &
                        ~(board.bitboards[turn]);
            break;
        }
        case 'B': {
            res = get_bishop_attacks(square, board.bitboards[both]) &
                        ~(board.bitboards[turn]);
            break;
        }
        case 'Q': {
            res = get_queen_attacks(square, board.bitboards[both]) &
                        ~(board.bitboards[turn]);
            break;
        }
    default:{
        break;
    }
    }
    return res;
}

/*
=======================================================================================================================================================================================================
                Update unsafe tile function is used to update two bitboards which contains info about all the tiles which are bieng attacked by the enemy

                this method is used to maintain the unsafe tiles bitboards which are used to generate legal king moves as king can't move to a tile which is bieng attacked by enemy
=======================================================================================================================================================================================================
*/
void Piece::update_unsafe_tiles(Board board){

    unsafe_tiles[white] = unsafe_tiles[black] = 0ULL;
    
    // find all the possible moves of white and add it to unsafe_tiles for black and vice versa
    for(int i = P; i<=k; i++){
        BitBoard piece_bitboard(this->piece_set[i].val);
        while (piece_bitboard.val){
            bool turn = (i<=K)? 1: 0;
            int source_tile = piece_bitboard.get_lsb_index();
            unsafe_tiles[turn] |= get_pseudo_legal_move(board, ascii_pieces[i], source_tile);
            piece_bitboard.pop_bit(source_tile);
        }
    }
}

bool Piece::is_king_safe(bool turn){
    return !((1ULL << kingPosition[turn]) & unsafe_tiles[turn]);
}

char Piece::is_there_piece(int tile){
    for (int i = P; i <= k; i++)
        if((1ULL<<tile) & piece_set[i].val) return ascii_pieces[i];
    
    return '0';
}

/*
=======================================================================================================================================================================================================
                    This function takes all the pseudo legal moves, make those moves on logical board and check if king is safe afther that move
                    if king is safe then that move is considered legal
=======================================================================================================================================================================================================
*/
uint64 Piece::get_legal_moves(Board board, char type, int source){
    int turn = (type < 'Z')?0:1;
    int piece = char_pieces.at(type);
    uint64 temp_unsafe[2];
    temp_unsafe[white] = unsafe_tiles[white];
    temp_unsafe[black] = unsafe_tiles[black];

    uint64 bitboards[3];
    bitboards[white] = board.bitboards[white];
    bitboards[black] = board.bitboards[black];
    bitboards[both] = board.bitboards[both];

    BitBoard piece_bitboard(this->piece_set[piece].val);

    int temp_king_position[2];
    temp_king_position[white] = kingPosition[white];
    temp_king_position[black] = kingPosition[black];

    // find the possible moves
    BitBoard possible_moves(get_pseudo_legal_move(board, type, source));

    uint64 valid_moves = 0ULL;
    while(possible_moves.val){
        int x = possible_moves.get_lsb_index();
        char captured = is_there_piece(x);
        uint64 captured_bitboard = 0ULL;

        if(captured != '0'){ // if the possible move is capture of enemy piece
            captured_bitboard = piece_set[char_pieces.at(captured)].val;
            update_piece_bitboard(captured, x, x);
        }

        // if king is bieng moved then update the king position as well
        if(type == 'k' || type == 'K') (type == 'K')? kingPosition[white] = x: kingPosition[black] = x;

        // update all the variables representing the state of board and pieceSet according to move
        update_piece_bitboard(type, source, x);
        board.sync_bitboards(this->piece_set);
        update_unsafe_tiles(board);

        //if king is safe after executing the current possible move then consider this as a valid move
        if(is_king_safe(turn)) valid_moves = valid_moves | (1ULL << x); 

        // reset everything;
        if(captured != '0') piece_set[char_pieces.at(captured)].val = captured_bitboard;

        unsafe_tiles[white] = temp_unsafe[white];
        unsafe_tiles[black] = temp_unsafe[black];

        board.bitboards[white] = bitboards[white];
        board.bitboards[black] = bitboards[black];
        board.bitboards[both] = bitboards[both];

        this->piece_set[piece].val = piece_bitboard.val;

        kingPosition[white] = temp_king_position[white];
        kingPosition[black] = temp_king_position[black];
        possible_moves.pop_bit(x);
    }
    return valid_moves;
}

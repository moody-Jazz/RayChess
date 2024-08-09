#include "../include/piece.hpp"
#include "../include/chessboard.hpp"

Piece::Piece(){
    piece_set[P].set_val(65280ULL);
    /*  White pawn bitboard
        8  0 0 0 0 0 0 0 0 
        7  0 0 0 0 0 0 0 0 
        6  0 0 0 0 0 0 0 0 
        5  0 0 0 0 0 0 0 0 
        4  0 0 0 0 0 0 0 0 
        3  0 0 0 0 0 0 0 0
        2  1 1 1 1 1 1 1 1 
        1  0 0 0 0 0 0 0 0

        a b c d e f g h
    */
    piece_set[N].set_val(66ULL);
    /*  white knight bitboard
        8  0 0 0 0 0 0 0 0 
        7  0 0 0 0 0 0 0 0 
        6  0 0 0 0 0 0 0 0 
        5  0 0 0 0 0 0 0 0 
        4  0 0 0 0 0 0 0 0 
        3  0 0 0 0 0 0 0 0
        2  0 0 0 0 0 0 0 0
        1  0 1 0 0 0 0 1 0 

           a b c d e f g h
    */
   piece_set[B].set_val(36ULL);
    /*  white bishop bitboard
        8  0 0 0 0 0 0 0 0 
        7  0 0 0 0 0 0 0 0 
        6  0 0 0 0 0 0 0 0 
        5  0 0 0 0 0 0 0 0 
        4  0 0 0 0 0 0 0 0 
        3  0 0 0 0 0 0 0 0
        2  0 0 0 0 0 0 0 0
        1  0 0 1 0 0 1 0 0
        
           a b c d e f g h
    */
    piece_set[R].set_val(129ULL);
    /*  white rook bitboard
        8  0 0 0 0 0 0 0 0 
        7  0 0 0 0 0 0 0 0 
        6  0 0 0 0 0 0 0 0 
        5  0 0 0 0 0 0 0 0 
        4  0 0 0 0 0 0 0 0 
        3  0 0 0 0 0 0 0 0
        2  0 0 0 0 0 0 0 0
        1  1 0 0 0 0 0 0 1
        
           a b c d e f g h
    */
    piece_set[Q].set_val(16ULL);
    /*  white queen bitboard
        8  0 0 0 0 0 0 0 0 
        7  0 0 0 0 0 0 0 0 
        6  0 0 0 0 0 0 0 0 
        5  0 0 0 0 0 0 0 0 
        4  0 0 0 0 0 0 0 0 
        3  0 0 0 0 0 0 0 0
        2  0 0 0 0 0 0 0 0
        1  0 0 0 1 0 0 0 0
        
           a b c d e f g h
    */
    piece_set[K].set_val(8ULL);
    /*  white king bitboard
        8  0 0 0 0 0 0 0 0 
        7  0 0 0 0 0 0 0 0 
        6  0 0 0 0 0 0 0 0 
        5  0 0 0 0 0 0 0 0 
        4  0 0 0 0 0 0 0 0 
        3  0 0 0 0 0 0 0 0
        2  0 0 0 0 0 0 0 0
        1  0 0 0 0 1 0 0 0
        
           a b c d e f g h
    */
    piece_set[p].set_val(71776119061217280ULL);
    /*  black pawn bitboard
        8  0 0 0 0 0 0 0 0 
        7  1 1 1 1 1 1 1 1 
        6  0 0 0 0 0 0 0 0 
        5  0 0 0 0 0 0 0 0 
        4  0 0 0 0 0 0 0 0 
        3  0 0 0 0 0 0 0 0
        2  0 0 0 0 0 0 0 0
        1  0 0 0 0 0 0 0 0
        
           a b c d e f g h
    */
    piece_set[n].set_val(4755801206503243776ULL);
    /*  black knight bitboard
        8  0 1 0 0 0 0 1 0 
        7  0 0 0 0 0 0 0 0 
        6  0 0 0 0 0 0 0 0 
        5  0 0 0 0 0 0 0 0 
        4  0 0 0 0 0 0 0 0 
        3  0 0 0 0 0 0 0 0
        2  0 0 0 0 0 0 0 0
        1  0 0 0 0 0 0 0 0
        
           a b c d e f g h
    */
    piece_set[b].set_val(2594073385365405696ULL);
    /*  black bishop bitboard
        8  0 0 1 0 0 1 0 0 
        7  0 0 0 0 0 0 0 0 
        6  0 0 0 0 0 0 0 0 
        5  0 0 0 0 0 0 0 0 
        4  0 0 0 0 0 0 0 0 
        3  0 0 0 0 0 0 0 0
        2  0 0 0 0 0 0 0 0
        1  0 0 0 0 1 0 0 0
        
           a b c d e f g h
    */
    piece_set[r].set_val(9295429630892703744ULL);
    /*  black rook bitboard
        8  1 0 0 0 0 0 0 1 
        7  0 0 0 0 0 0 0 0 
        6  0 0 0 0 0 0 0 0 
        5  0 0 0 0 0 0 0 0 
        4  0 0 0 0 0 0 0 0 
        3  0 0 0 0 0 0 0 0
        2  0 0 0 0 0 0 0 0
        1  0 0 0 0 1 0 0 0
        
           a b c d e f g h
    */
    piece_set[q].set_val(1152921504606846976ULL);
    /*  black queen bitboard
        8  0 0 0 1 0 0 0 0 
        7  0 0 0 0 0 0 0 0 
        6  0 0 0 0 0 0 0 0 
        5  0 0 0 0 0 0 0 0 
        4  0 0 0 0 0 0 0 0 
        3  0 0 0 0 0 0 0 0
        2  0 0 0 0 0 0 0 0
        1  0 0 0 0 1 0 0 0
        
           a b c d e f g h
    */
    piece_set[k].set_val(576460752303423488ULL);
    /*  black king bitboard
        8  0 0 0 0 1 0 0 0 
        7  0 0 0 0 0 0 0 0 
        6  0 0 0 0 0 0 0 0 
        5  0 0 0 0 0 0 0 0 
        4  0 0 0 0 0 0 0 0 
        3  0 0 0 0 0 0 0 0
        2  0 0 0 0 0 0 0 0
        1  0 0 0 0 0 0 0 0
        
           a b c d e f g h
    */
}

BitBoard pawn_attack_bitmask(int side, int square){
    
    // result attacks bitboard
    BitBoard attacks(0ULL);

    // piece bitboard
    BitBoard bitboard(0ULL);

    //the current position of piece
    bitboard.set_bit(square);

    //white pawns
    if(!side){
        if((bitboard.val >> 7) & not_a_file) attacks.val |= (bitboard.val >> 7);
        if((bitboard.val >> 9) & not_h_file) attacks.val |= (bitboard.val >> 9);
    }
    //black pawns
    else{
        if((bitboard.val << 7) & not_h_file) attacks.val |= (bitboard.val << 7);
        if((bitboard.val << 9) & not_a_file) attacks.val |= (bitboard.val << 9);
    }
    // return attack bitboard
    return attacks;
}
//generate knight attacks
BitBoard Piece::get_knight_attacks(int square){
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
    return attacks;
}
//generate king attacks
BitBoard Piece::get_king_attacks(int square){
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
    return attacks;
}
BitBoard Piece::get_bishop_attacks(int square, BitBoard block){
    BitBoard attacks(0ULL);
    int r, f;
    int tr = square/8;
    int tf = square % 8; 

    for(r = tr +1, f = tf+1; r<=7 && f<=7; r++, f++){
        attacks.val |= (1ULL << (r*8+f));
        if(block.val & (1ULL << (r*8+f))) break;
    } 
    for(r = tr -1, f = tf+1; r>=0 && f<=7; r--, f++){
        attacks.val |= (1ULL << (r*8+f));
        if(block.val & (1ULL << (r*8+f))) break;
    }
    for(r = tr +1, f = tf-1; r<=7 && f>=0; r++, f--){
        attacks.val |= (1ULL << (r*8+f));
        if(block.val & (1ULL << (r*8+f))) break;
    }
    for(r = tr -1, f = tf-1; r>=0 && f>=0; r--, f--){
        attacks.val |= (1ULL << (r*8+f));
        if(block.val & (1ULL << (r*8+f))) break;
    } 

    return attacks;
}
BitBoard Piece::get_rook_attacks(int square, BitBoard block){
    BitBoard attacks(0ULL);

    int r,f;
    int tr = square / 8;
    int tf = square % 8;

    for(r = tr+1; r<= 7; r++) {
        attacks.val |= (1ULL << (r*8 +tf));
        if((1ULL << (r*8 +tf)) & block.val) break;

    }
    for(r = tr-1; r>= 0; r--) {
        attacks.val |= (1ULL << (r*8 +tf));
        if((1ULL << (r*8 +tf)) & block.val) break;
    }
    for(f = tf + 1; f<=7; f++) {
        attacks.val |= (1ULL << (tr *8 +f));
        if((1ULL << (tr *8 +f)) & block.val) break;
    }
    for(f = tf - 1; f>=0; f--) {
        attacks.val |= (1ULL << (tr *8 +f));
        if((1ULL << (tr *8 +f)) & block.val) break;
    }
    return attacks.val;
}
BitBoard Piece::get_queen_attacks(int square, BitBoard block){
    BitBoard attacks(0ULL);
    attacks.val |= get_bishop_attacks(square, block).val;
    attacks.val |= get_rook_attacks(square, block).val;
    return attacks;
}
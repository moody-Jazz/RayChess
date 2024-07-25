#include<iostream>
using namespace std;



//defines an unsigned long long as u64********************************************************************
#define u64 unsigned long long 


// define macros***********************************************************************************
#define get_bit(bitboard, square) (bitboard & (1ULL <<square))
#define set_bit(bitboard,square) (bitboard |= (1ULL << square))
#define pop_bit(bitboard, square) (get_bit(bitboard, square)? bitboard^=(1ULL<< square):0)

// enum definitions ***********************************************************************************************
enum{
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1
};

enum{white, black};

//print the bit board***********************************************************************
void print_bb(u64 bitboard){
    for(int rank{} ; rank<8; rank++){
        for(int file{}; file<8; file++){
            int square = file+rank*8;
            if(!file)
                cout<< 8-rank<<"  ";
            bool x = (get_bit(bitboard,square))? 1: 0;  
            cout<< x << " ";
        }
        cout<<endl;
    }
    cout<<endl<<"   a b c d e f g h"<<endl<<endl;

    //print bitboard as unsigned decimal number
    cout<<"bitboard value: "<<bitboard<<endl;
   
}

// attack tables**********************************************************************************

/*  
not file a :
    8  0 1 1 1 1 1 1 1
    7  0 1 1 1 1 1 1 1
    6  0 1 1 1 1 1 1 1
    5  0 1 1 1 1 1 1 1
    4  0 1 1 1 1 1 1 1
    3  0 1 1 1 1 1 1 1
    2  0 1 1 1 1 1 1 1
    1  0 1 1 1 1 1 1 1
       a b c d e f g h
*/const u64 not_a_file = 18374403900871474942ULL;
/*  
not_h_file: 
    8  1 1 1 1 1 1 1 0
    7  1 1 1 1 1 1 1 0
    6  1 1 1 1 1 1 1 0
    5  1 1 1 1 1 1 1 0
    4  1 1 1 1 1 1 1 0
    3  1 1 1 1 1 1 1 0
    2  1 1 1 1 1 1 1 0
    1  1 1 1 1 1 1 1 0
       a b c d e f g h
*/const u64 not_h_file = 9187201950435737471ULL;
/*  
not_hg_file:
    8  1 1 1 1 1 1 0 0
    7  1 1 1 1 1 1 0 0
    6  1 1 1 1 1 1 0 0
    5  1 1 1 1 1 1 0 0
    4  1 1 1 1 1 1 0 0 
    3  1 1 1 1 1 1 0 0
    2  1 1 1 1 1 1 0 0
    1  1 1 1 1 1 1 0 0
       a b c d e f g h
*/const u64 not_hg_file = 4557430888798830399ULL;
/*  
not_ab_file:
    8  0 0 1 1 1 1 1 1
    7  0 0 1 1 1 1 1 1
    6  0 0 1 1 1 1 1 1
    5  0 0 1 1 1 1 1 1
    4  0 0 1 1 1 1 1 1
    3  0 0 1 1 1 1 1 1
    2  0 0 1 1 1 1 1 1
    1  0 0 1 1 1 1 1 1
       a b c d e f g h
*/const u64 not_ab_file = 18229723555195321596ULL;

//*******************************************************************************************************

// attack table
u64 pawn_attacks[2][64];
u64 knight_attacks[64];
u64 king_attacks[64];
u64 rook_attacks[64];
u64 bishop_attacks[64];
u64 queen_attacks[64];

// generate pawn attacks
u64 pawn_attack_bitmask(int side, int square){
    
    // result attacks bitboard
    u64 attacks = 0ULL;

    // piece bitboard
    u64 bitboard = 0ULL;

    //the current position of piece
    set_bit(bitboard, square);

    //white pawns
    if(!side){
        if((bitboard >> 7) & not_a_file) attacks |= (bitboard >> 7);
        if((bitboard >> 9) & not_h_file) attacks |= (bitboard >> 9);
    }
    //black pawns
    else{
        if((bitboard << 7) & not_h_file) attacks |= (bitboard << 7);
        if((bitboard << 9) & not_a_file) attacks |= (bitboard << 9);
    }
    // return attack map
    return attacks;

}
//generate knight attacks
u64 knight_attack_bitmask(int square){
    u64 attacks = 0ULL;

    u64 bitboard = 0ULL;

    // set piece position
    set_bit(bitboard, square);

    //generate knight attacks
    if((bitboard >> 17) & not_h_file) attacks |= (bitboard >> 17);
    if((bitboard >> 15) & not_a_file) attacks |= (bitboard >> 15);
    if((bitboard >> 10) & not_hg_file) attacks |= (bitboard >> 10);
    if((bitboard >> 6) & not_ab_file) attacks |= (bitboard >> 6);    

    if((bitboard << 17) & not_a_file) attacks |= (bitboard << 17);
    if((bitboard << 15) & not_h_file) attacks |= (bitboard << 15);
    if((bitboard << 10) & not_ab_file) attacks |= (bitboard << 10);
    if((bitboard << 6) & not_hg_file) attacks |= (bitboard << 6); 
    return attacks;
}
//generate king attacks
u64 king_attack_bitmask(int square){
    u64 attacks = 0ULL;

    u64 bitboard = 0ULL; 

    set_bit(bitboard, square);

    if(bitboard >> 8) attacks |= (bitboard >> 8);
    if((bitboard >> 9) & not_h_file) attacks |= (bitboard >> 9);
    if((bitboard >> 7) & not_a_file) attacks |= (bitboard >> 7);
    if((bitboard >> 1) & not_h_file)attacks |= (bitboard >> 1);  

    if(bitboard << 8) attacks |= (bitboard << 8);
    if((bitboard << 9) & not_a_file) attacks |= (bitboard << 9);
    if((bitboard << 7) & not_h_file) attacks |= (bitboard << 7);
    if((bitboard << 1) & not_a_file)attacks |= (bitboard << 1);  
    
    return attacks;
}
//generate bishop attacks
u64 bishop_attack_bitmask(int square){
    u64 attacks = 0ULL;
    int r, f;
    int tr = square/8;
    int tf = square % 8; 

    for(r = tr +1, f = tf+1; r<=6 && f<=6; r++, f++){
        attacks |= (1ULL << (r*8+f));
    } 
    for(r = tr -1, f = tf+1; r>=1 && f<=6; r--, f++){
        attacks |= (1ULL << (r*8+f));
    }
    for(r = tr +1, f = tf-1; r<=6 && f>=1; r++, f--){
        attacks |= (1ULL << (r*8+f));
    }
    for(r = tr -1, f = tf-1; r>=1 && f>=1; r--, f--){
        attacks |= (1ULL << (r*8+f));
    } 

    return attacks;
}
u64 bishop_attack_onthefly(int square, u64 block){
    u64 attacks = 0ULL;
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
//generate rook attacks
u64 rook_attack_bitmask(int square){
    u64 attacks = 0ULL;

    int r,f;
    int tr = square / 8;
    int tf = square % 8;

    for(r = tr+1; r<= 6; r++) attacks |= (1ULL << (r*8 +tf));
    for(r = tr-1; r>= 1; r--) attacks |= (1ULL << (r*8 +tf));
    for(f = tf + 1; f<=6; f++) attacks |= (1ULL << (tr *8 +f));
    for(f = tf - 1; f>=1; f--) attacks |= (1ULL << (tr *8 +f));
    return attacks;
}
u64 rook_attack_onthefly(int square, u64 block){
    u64 attacks = 0ULL;

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

void init_leapers_attacks(){
    //loop over every sqare
    for(int square{}; square<64; square++){
        // init pawn attack bitmask here
        pawn_attacks[white][square] = pawn_attack_bitmask(white, square);
        pawn_attacks[black][square] = pawn_attack_bitmask(black, square);

        // init knight attacks
        knight_attacks[square] = knight_attack_bitmask(square);

        // init king attacks
        king_attacks[square] = king_attack_bitmask(square);
        // init bishop attacks
        // bishop_attacks[square] = bishop_attack_bitmask(square);
        // // init rook attacks
        // rook_attacks[square] = rook_attack_bitmask(square);
    }
}
//*******************************************main********************************************
int main(){
    init_leapers_attacks();
    u64 block = 0ULL;
    set_bit(block, e5);
    print_bb(rook_attack_onthefly(e3, block));
    // for(int rank{} ; rank<8; rank++){
    //     for(int file{}; file<8; file++){
    //         int square = file+rank*8;
    //         cout<<couint_bits(bishop_attack_bitmask(square));
    //     }
    //     cout<<endl;
    // }
    // cout<<endl<<"   a b c d e f g h"<<endl<<endl;
    // for(int i = 8; i>=1; i--)
    //cout<<'a'<<i<<", "<<'b'<<i<<", "<<'c'<<i<<", "<<'d'<<i<<", "<<'e'<<i<<", "<<'f'<<i<<", "<<'g'<<i<<", "<<'h'<<i<<", " <<endl;
    
}
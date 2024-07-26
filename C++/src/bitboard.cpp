#include<bits/stdc++.h>
#include<stdio.h>
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
std::string coordinate[] = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7"," h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
};
enum{white, black};
enum{rook, bishop};

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
//***********************************************************************************************************

static inline int count_bits(u64 bitboard){
    int count{};
    // keep reseting least significatn 1st bit until bitboard is 0
    while(bitboard){
        bitboard = bitboard & bitboard -1;
        count++;
    }
    return count;
}
static inline int get_lsb_index(u64 bitboard){
    return bitboard ? count_bits((bitboard & -bitboard) - 1) : -1;
}
// ***********************************************************************************************

u64 set_occupency(int index, int mask_bitcount, u64 attack_mask){
    // occupancy map
    u64 occupancy = 0ULL;
    for(int count{}; count<mask_bitcount; count++){
        int square = get_lsb_index(attack_mask);

        pop_bit(attack_mask, square); // reset first bit in attack map
        if(index & (1<<count))  occupancy |= (1ULL << square);
    }

    return occupancy;
}
//*******************************************main********************************************
// tables for all occupancy bit count on all possible square
const int bishop_occupancy_table[64] = {
    6, 5, 5, 5, 5, 5, 5, 6, 
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5, 
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};
const int rook_occupancy_table[64] = {
    12, 11, 11, 11, 11, 11, 11, 12, 
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};

// rook magic numbars
u64 rook_magic_numbers[64];

// bishop magic numbers
u64 bishop_magic_numbers[64];
////////////////////////////////////Random magic number////////////////////////////////////////////////////////
// pseudo random number state
unsigned int random_state = 1804289383;
// generate 32-bit pseudo legal number
unsigned int get_rand_u32_num(){
    unsigned int num = random_state;
    // xor shift algo
    num ^= num << 13;
    num ^= num >> 17;
    num ^= num << 5;
    // update random number state
    random_state = num;
    return num;
}
u64 get_rand_u64_num(){
    u64 n1,n2,n3,n4;
    n1 = (u64)(get_rand_u32_num() & 0xFFFF);// slicing upper 16 bits
    n2 = (u64)(get_rand_u32_num() & 0xFFFF);
    n3 = (u64)(get_rand_u32_num() & 0xFFFF);
    n4 = (u64)(get_rand_u32_num() & 0xFFFF);

    return n1 | (n2<<16) | (n3<<32) | (n4<<48);
}
u64 generate_magic_num(){
    return get_rand_u64_num() & get_rand_u64_num() & get_rand_u64_num();
}


///////////////////////////////////////Maigcs//////////////////////////////////////////////////////////

// find appropriate magic number
u64 find_magic_num(int square, int relevant_bits, int bishop){
    // init occupancies
    u64 occupancies[4096]; // array of size 4096 can fit number of occupancies for rook as well as bishop
    // init attack tables
    u64 attacks[4096];

    // init used attacks
    u64 used_attacks[4096];

    // init attack mask for a current piece
    u64 attack_mask = bishop? bishop_attack_bitmask(square): rook_attack_bitmask(square);
    
    // init occupancy indices
    int occupancy_indicies = 1 << relevant_bits;

    // loop over occupancy indicies
    for(int ind{}; ind<occupancy_indicies; ind++){
        // init occupancies
        occupancies[ind] = set_occupency(ind, relevant_bits, attack_mask);
        // init attacks
        attacks[ind] = bishop? bishop_attack_onthefly(square, occupancies[ind]):
                                rook_attack_onthefly(square, occupancies[ind]);
    }
    // test magic numbers loop
    for(int random_count{}; random_count < 100000000; random_count++){
        //generate magic number candidate
        u64 magic_number = generate_magic_num();
        // skip inappropriate magic numbers
        if(count_bits((attack_mask* magic_number) & 0xFF00000000000000) < 6) continue;
        //init used attacks
        memset(used_attacks, 0ULL, sizeof(used_attacks));

        // init index & fail flag
        int index, fail;
        // init index and fail flag
        for(index = 0, fail = 0; !fail && index < occupancy_indicies; index++){
            // init magic index
            int magic_index = (int) ((occupancies[index] * magic_number) >> (64 - relevant_bits));
            // if magic index works
            if(used_attacks[magic_index] == 0ULL)
                used_attacks[magic_index] = attacks[index]; // init used attacks
            else if(used_attacks[magic_index] != attacks[index])
                fail = 1;
        }
        if(!fail) return magic_number;
    }
    cout<<"magic number fails"<<endl;
    return 0ULL;
}   
//////////////////////////////////////////////////////////////////////////////////////////////


// init magic number
void init_magic_num(){
    // loop over 64 boards square print three magic numbers and then endline
    int count{};
    for(int square{}; square<64; square++){
        count++;
        printf(" 0x%llxULL, ",find_magic_num(square, rook_occupancy_table[square], rook));
    if(count == 3){cout<<endl; count = 0;}
    }
    cout<<endl<<endl<<endl;
    count = 0;
    for(int square{}; square<64; square++){
        count++;
        printf(" 0x%llxULL, ",find_magic_num(square, bishop_occupancy_table[square], bishop));
        if(count == 3){cout<<endl; count = 0;}
    }
}
int main(){
    init_leapers_attacks();
    print_bb(generate_magic_num());
    init_magic_num();
    // for(int rank{} ; rank<8; rank++){
    //     for(int file{}; file<8; file++){
    //         int square = file+rank*8;
    //         cout<<count_bits(rook_attack_bitmask(square))<<", ";
    //     }
    //     cout<<endl;
    // }
    // cout<<endl<<"   a b c d e f g h"<<endl<<endl;
}
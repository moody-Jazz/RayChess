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
u64 rook_attacks[64][4096];
u64 bishop_attacks[64][512];
u64 queen_attacks[64];
u64 bishop_masks[64];
u64 rook_masks[64];

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


/* init magic number (function used to initialze the rook_magic_numbers array and bishop_magic_numbers array)
void init_magic_num(){
    // loop over 64 boards square
    for(int square{}; square<64; square++){
        rook_magic_numbers[square] = find_magic_num(square, rook_occupancy_table[square], rook);
        bishop_magic_numbers[square] = find_magic_num(square, bishop_occupancy_table[square], bishop);
    }
} */

// rook magic numbars
const u64 rook_magic_numbers[64] = {
    0x8a80104000800020ULL,  0x140002000100040ULL,  0x2801880a0017001ULL, 
    0x100081001000420ULL,  0x200020010080420ULL,  0x3001c0002010008ULL, 
    0x8480008002000100ULL,  0x2080088004402900ULL,  0x800098204000ULL, 
    0x2024401000200040ULL,  0x100802000801000ULL,  0x120800800801000ULL, 
    0x208808088000400ULL,  0x2802200800400ULL,  0x2200800100020080ULL, 
    0x801000060821100ULL,  0x80044006422000ULL,  0x100808020004000ULL, 
    0x12108a0010204200ULL,  0x140848010000802ULL,  0x481828014002800ULL, 
    0x8094004002004100ULL,  0x4010040010010802ULL,  0x20008806104ULL, 
    0x100400080208000ULL,  0x2040002120081000ULL,  0x21200680100081ULL, 
    0x20100080080080ULL,  0x2000a00200410ULL,  0x20080800400ULL, 
    0x80088400100102ULL,  0x80004600042881ULL,  0x4040008040800020ULL, 
    0x440003000200801ULL,  0x4200011004500ULL,  0x188020010100100ULL, 
    0x14800401802800ULL,  0x2080040080800200ULL,  0x124080204001001ULL, 
    0x200046502000484ULL,  0x480400080088020ULL,  0x1000422010034000ULL, 
    0x30200100110040ULL,  0x100021010009ULL,  0x2002080100110004ULL, 
    0x202008004008002ULL,  0x20020004010100ULL,  0x2048440040820001ULL, 
    0x101002200408200ULL,  0x40802000401080ULL,  0x4008142004410100ULL, 
    0x2060820c0120200ULL,  0x1001004080100ULL,  0x20c020080040080ULL, 
    0x2935610830022400ULL,  0x44440041009200ULL,  0x280001040802101ULL, 
    0x2100190040002085ULL,  0x80c0084100102001ULL,  0x4024081001000421ULL, 
    0x20030a0244872ULL,  0x12001008414402ULL,  0x2006104900a0804ULL, 
    0x1004081002402ULL
};
// bishop magic numbers
const u64 bishop_magic_numbers[64] = {
    0x40040844404084ULL,  0x2004208a004208ULL,  0x10190041080202ULL,
    0x108060845042010ULL,  0x581104180800210ULL,  0x2112080446200010ULL, 
    0x1080820820060210ULL,  0x3c0808410220200ULL,  0x4050404440404ULL,
    0x21001420088ULL,  0x24d0080801082102ULL,  0x1020a0a020400ULL,
    0x40308200402ULL,  0x4011002100800ULL,  0x401484104104005ULL,
    0x801010402020200ULL,  0x400210c3880100ULL,  0x404022024108200ULL,
    0x810018200204102ULL,  0x4002801a02003ULL,  0x85040820080400ULL,
    0x810102c808880400ULL,  0xe900410884800ULL,  0x8002020480840102ULL,
    0x220200865090201ULL,  0x2010100a02021202ULL,  0x152048408022401ULL,
    0x20080002081110ULL,  0x4001001021004000ULL,  0x800040400a011002ULL,
    0xe4004081011002ULL,  0x1c004001012080ULL,  0x8004200962a00220ULL,
    0x8422100208500202ULL,  0x2000402200300c08ULL,  0x8646020080080080ULL,
    0x80020a0200100808ULL,  0x2010004880111000ULL,  0x623000a080011400ULL,
    0x42008c0340209202ULL,  0x209188240001000ULL,  0x400408a884001800ULL,
    0x110400a6080400ULL,  0x1840060a44020800ULL,  0x90080104000041ULL,
    0x201011000808101ULL,  0x1a2208080504f080ULL,  0x8012020600211212ULL,
    0x500861011240000ULL,  0x180806108200800ULL,  0x4000020e01040044ULL,
    0x300000261044000aULL,  0x802241102020002ULL,  0x20906061210001ULL,
    0x5a84841004010310ULL,  0x4010801011c04ULL,  0xa010109502200ULL,
    0x4a02012000ULL,  0x500201010098b028ULL,  0x8040002811040900ULL,
    0x28000010020204ULL,  0x6000020202d0240ULL,  0x8918844842082200ULL,
    0x4010011029020020ULL
};
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

//***********************************************************************************************************



// ***********************************************************************************************

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

void init_slider_attacks(int bishop){
    for(int square{}; square<64; square++){
        // init bishop and rook masks
        bishop_masks[square] = bishop_attack_bitmask(square);
        rook_masks[square] = rook_attack_bitmask(square);

        // init current masks
        u64 attack_mask = bishop? bishop_masks[square]: rook_masks[square];
        int relevant_bit_count = count_bits(attack_mask);
        // init occupancy indicies
        int ocupancy_indicies = 1<< relevant_bit_count;

        for(int index{}; index < ocupancy_indicies; index++){
            if(bishop){
                u64 occupancy = set_occupency(index, relevant_bit_count, attack_mask);
                int magic_index = (occupancy * bishop_magic_numbers[square]) >> (64 -bishop_occupancy_table[square] );
                bishop_attacks[square][magic_index] = bishop_attack_onthefly(square, occupancy);

            }
            else{
                u64 occupancy = set_occupency(index, relevant_bit_count, attack_mask);
                int magic_index = (occupancy * rook_magic_numbers[square]) >> (64 -rook_occupancy_table[square] );
                rook_attacks[square][magic_index] = rook_attack_onthefly(square, occupancy);
            }
        }
    }
}

static inline u64 get_bishop_attacks(int square, u64 occupancy){
    // get bishop attacks assuming current board occupancy
    occupancy &= bishop_masks[square];
    occupancy *= bishop_magic_numbers[square];
    occupancy >>= 64-bishop_occupancy_table[square];
    return bishop_attacks[square][occupancy];
}
static inline u64 get_rook_attacks(int square, u64 occupancy){
    // get rook attacks assuming current board occupancy
    occupancy &= rook_masks[square];
    occupancy *= rook_magic_numbers[square];
    occupancy >>= 64 - rook_occupancy_table[square];
    return rook_attacks[square][occupancy];
}
void init_all(){
    init_leapers_attacks();
    init_slider_attacks(bishop);
    init_slider_attacks(rook);
}
int main(){
    init_all();
    u64 occupancy = 0ULL; 
    set_bit(occupancy, d6);
    print_bb(occupancy);
    print_bb(get_rook_attacks(d4, occupancy));
    // for(int rank{} ; rank<8; rank++){
    //     for(int file{}; file<8; file++){
    //         int square = file+rank*8;
    //         cout<<count_bits(rook_attack_bitmask(square))<<", ";
    //     }
    //     cout<<endl;
    // }
    // cout<<endl<<"   a b c d e f g h"<<endl<<endl;
}
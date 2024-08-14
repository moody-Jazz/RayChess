#pragma once
#include <string>
#include "../include/bitboard.hpp"
#include <unordered_map>

#define uint64 unsigned long long


/*
Below are the absolute values of bitmask that is required to find the pseudo legal move of a piece 
for example a white pawn that is on b2 will have attack bitmask as 

        8  0 0 0 0 0 0 0 0 
        7  0 0 0 0 0 0 0 0 
        6  0 0 0 0 0 0 0 0 
        5  0 0 0 0 0 0 0 0 
        4  0 0 0 0 0 0 0 0 
        3  1 0 1 0 0 0 0 0
        2  0 P 0 0 0 0 0 0
        1  0 0 0 0 0 0 0 0
           a b c d e f g h

this bitmask can be used to perform some bitmanipulation with the enmy piece bitboard and the friendly piece bitboard to find
the pseudo legal move. 
i have used some algorithms from piece.cpp which do bitmanipulation to geenrate all this numbers for example to find knight moves i have used:
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
    this algorithm takes the position of the knight and returns a number (attacks) which have the attacking squares set as 1 bit
    then i printed the binary form and decimal value of attacks using the print_binary() method defined in bitboard.cpp

    the process mentioned above have been used for generating all the attack bitmask and then storing them so that we dont have 
    to generate them on the fly at the expense of loosing speed

*/

// the pawn attack bitmask is only of size 56 and not 64 is because we dont need the last 8 bits as the black pawns start from 
// the 9th bit from left and white pawns promote after reaching the last rank
static const uint64 pawn_attack_bitmask[2][56] = {
    {512, 1280, 2560, 5120, 10240, 20480, 40960, 16384, 131072, 327680, 655360, 1310720,
     2621440, 5242880, 10485760, 4194304, 33554432, 83886080, 167772160, 335544320,
     671088640, 1342177280, 2684354560, 1073741824, 8589934592, 21474836480, 42949672960,
     85899345920, 171798691840, 343597383680, 687194767360, 274877906944, 2199023255552,
     5497558138880, 10995116277760, 21990232555520, 43980465111040, 87960930222080,
     175921860444160, 70368744177664, 562949953421312, 1407374883553280, 2814749767106560,
     5629499534213120, 11258999068426240, 22517998136852480, 45035996273704960,
     18014398509481984, 144115188075855872, 360287970189639680, 720575940379279360,
     1441151880758558720, 2882303761517117440, 5764607523034234880, 11529215046068469760,
     4611686018427387904},

    {0, 0, 0, 0, 0, 0, 0, 0, 2, 5, 10, 20, 40, 80, 160, 64, 512, 1280, 2560, 5120, 10240, 20480,
     40960, 16384, 131072, 327680, 655360, 1310720, 2621440, 5242880, 10485760, 4194304, 33554432,
     83886080, 167772160, 335544320, 671088640, 1342177280, 2684354560, 1073741824, 8589934592,
     21474836480, 42949672960, 85899345920, 171798691840, 343597383680, 687194767360, 274877906944,
     2199023255552, 5497558138880, 10995116277760, 21990232555520, 43980465111040, 87960930222080,
     175921860444160, 70368744177664}};

static const uint64 pawn_push_bitmask[2][56] = {
    {256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 16842752, 33685504, 67371008, 134742016,
     269484032, 538968064, 1077936128, 2155872256, 16777216, 33554432, 67108864, 134217728,
     268435456, 536870912, 1073741824, 2147483648, 4294967296, 8589934592, 17179869184,
     34359738368, 68719476736, 137438953472, 274877906944, 549755813888, 1099511627776,
     2199023255552, 4398046511104, 8796093022208, 17592186044416, 35184372088832, 70368744177664,
     140737488355328, 281474976710656, 562949953421312, 1125899906842624, 2251799813685248,
     4503599627370496, 9007199254740992, 18014398509481984, 36028797018963968, 72057594037927936,
     144115188075855872, 288230376151711744, 576460752303423488, 1152921504606846976,
     2305843009213693952, 4611686018427387904, 9223372036854775808},

    {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384,
     32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432,
     67108864, 134217728, 268435456, 536870912, 1073741824, 2147483648, 4294967296, 8589934592,
     17179869184, 34359738368, 68719476736, 137438953472, 274877906944, 549755813888, 1103806595072,
     2207613190144, 4415226380288, 8830452760576, 17660905521152, 35321811042304, 70643622084608,
     141287244169216}};

static const uint64 knight_attack_bitmask[64] = {
    132096, 329728, 659712, 1319424, 2638848, 5277696, 10489856, 4202496, 33816580, 84410376, 168886289, 
    337772578, 675545156, 1351090312, 2685403152, 1075839008, 8657044482, 21609056261, 43234889994,
    86469779988, 172939559976, 345879119952, 687463207072, 275414786112, 2216203387392, 5531918402816,
    11068131838464, 22136263676928, 44272527353856, 88545054707712, 175990581010432, 70506185244672, 
    567348067172352, 1416171111120896, 2833441750646784, 5666883501293568, 11333767002587136,
    22667534005174272, 45053588738670592, 18049583422636032, 145241105196122112, 362539804446949376,
    725361088165576704, 1450722176331153408, 2901444352662306816, 5802888705324613632, 11533718717099671552,
    4620693356194824192, 288234782788157440, 576469569871282176, 1224997833292120064, 2449995666584240128,
    4899991333168480256, 9799982666336960512, 1152939783987658752, 2305878468463689728, 1128098930098176,
    2257297371824128, 4796069720358912, 9592139440717824, 19184278881435648, 38368557762871296,
    4679521487814656, 9077567998918656
};

static const uint64 king_attack_bitmask[64]{
    770, 1797, 3594, 7188, 14376, 28752, 57504, 49216, 197123, 460039, 920078, 1840156, 3680312, 7360624,  
    14721248, 12599488, 50463488, 117769984, 235539968, 471079936, 942159872, 1884319744, 3768639488,
    3225468928, 12918652928, 30149115904, 60298231808, 120596463616, 241192927232, 482385854464, 964771708928,
    825720045568, 3307175149568, 7718173671424, 15436347342848, 30872694685696, 61745389371392, 123490778742784,
    246981557485568, 211384331665408, 846636838289408, 1975852459884544, 3951704919769088, 7903409839538176,
    15806819679076352, 31613639358152704, 63227278716305408, 54114388906344448, 216739030602088448,
    505818229730443264, 1011636459460886528, 2023272918921773056, 4046545837843546112, 8093091675687092224,
    16186183351374184448, 13853283560024178688, 144959613005987840, 362258295026614272, 724516590053228544,
    1449033180106457088, 2898066360212914176, 5796132720425828352, 11592265440851656704, 4665729213955833856
};

class Piece
{
public:
    Piece();
    BitBoard piece_set[12];
    
    // leaper piece attack bitmasks
    BitBoard pawn_attack_bitmask(int square);
    BitBoard pawn_push_bitmask(int square);
    BitBoard knight_attack_bitmask(int square);
    BitBoard king_attack_bitmask(int square);

    // slider piece attack bitmask
    BitBoard get_bishop_attacks(int square, uint64 block);
    BitBoard get_rook_attacks(int square, uint64 block);
    BitBoard get_queen_attacks(int square, uint64 block);
    BitBoard get_legal_move();
};
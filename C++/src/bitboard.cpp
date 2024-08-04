#include <./include/bitboard.hpp>
#include <iostream>

using std::cout;
using std::endl;

inline bool BitBoard::get_bit(int square){
    return (bool)((bitboard) & (1ULL <<(square)));
}
inline void BitBoard::set_bit(int square){
    (bitboard) |= (1ULL << (square));
}
inline void BitBoard::pop_bit(int square){
    (bitboard) &= -(1ULL << (square));
}
inline void BitBoard::set_val(uint64 bitboard){
    this->bitboard = bitboard;
}

void BitBoard::print_binary(){
for(int rank{} ; rank<8; rank++){
        for(int file{}; file<8; file++){
            int square = file+rank*8;
            if(!file)
                cout<< 8-rank<<"  ";
            bool x = (get_bit(square))? 1: 0;  
            cout<< x << " ";
        }
        cout<<endl;
    }
    cout<<endl<<"   a b c d e f g h"<<endl<<endl;

    //print bitboard as unsigned decimal number
    cout<<"bitboard value: "<<bitboard<<endl;
}


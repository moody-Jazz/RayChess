#include "../include/bitboard.hpp"
#include <iostream>

using std::cout;
using std::endl;

BitBoard::BitBoard(){

}
BitBoard::BitBoard(uint64 val){
    this->val = val;
}

 bool BitBoard::get_bit(int square){
    return (bool)((this->val) & (1ULL <<(square )));
}
 void BitBoard::set_bit(int square){
    (this->val) |= (1ULL << (square ));
}
 void BitBoard::pop_bit(int square){
    (this->val)  &= ~(1ULL << (square));
}
 void BitBoard::set_val(uint64 bitboard){
    this->val = bitboard;
}

void BitBoard::print_binary(){
    int count = 0;
    for(int i = 63; i>=0; i--){
        cout<<get_bit(i)<<" ";
        count++;
        if(count == 8){
            cout<<"\n";
            count = 0;
        }
    }
    cout<<"    bitboard value: "<<val<<"\n";
// for(int rank = 7 ; rank>=0; rank--){
//     cout<<"    ";
//     for(int file = 7; file>=0; file--){
//         int square = file+rank*8;
//         bool x = (get_bit(square))? 1: 0;  
//         cout<<x << " ";
//     }
//     cout<<"\n";
}

 int BitBoard::count_bits(){
    int count{};
    uint64 bitboard = this->val;
    // keep reseting least significatn 1st bit until bitboard is 0
    while(bitboard){
        bitboard = bitboard & (bitboard -1);
        count++;
    }
    return count;
}
inline int BitBoard::get_lsb_index(){
    // to do 
    return 0;
}


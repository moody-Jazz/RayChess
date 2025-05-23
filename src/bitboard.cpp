#include "bitboard.hpp"
#include <iostream>
#include <cmath>

BitBoard::BitBoard(): val_(0ULL) {}

BitBoard::BitBoard(uint64_t val): val_(val) {}

uint64_t BitBoard::getVal() const
{
    return val_;
}

void BitBoard::printBinary() const
{
    size_t count = 0;
    for(int i = 63; i>=0; i--)
    {
        std::cout<<getBit(i)<<" ";
        count++;
        if(count == 8)
        {
            std::cout<<"\n";
            count = 0;
        }
    }
    std::cout<<"    bitboard value: "<<val_<<"\n";
}

bool BitBoard::getBit(size_t square) const
{
    return (val_) & (1ULL << square);
}

uint16_t BitBoard::getSetBitCount() const
{
    uint16_t count{};
    uint64_t bitboard = val_;
    // keep reseting least significatn 1st bit until bitboard is 0
    while(bitboard)
    {
        bitboard = bitboard & (bitboard -1);
        count++;
    }
    return count;
}

int BitBoard::getLSBIndex() const
{
    return val_ ? static_cast<int>(log2(val_ & -val_)): -1;
}

void BitBoard::setVal(uint64_t val)
{
   val_ = val;
}

void BitBoard::setBit(size_t square)
{
    // setting a specific the bit by doing OR operation with 1 at that specific index
    val_ |= (1ULL << square );
}

void BitBoard::popBit(size_t square)
{   
    // erasing a specific the bit by doing AND operation with 0 at that specific index
    val_  &= ~(1ULL << square);
}

BitBoard BitBoard::operator = (const BitBoard& obj)
{
    if (this != &obj)  // Check for self-assignment
        this->val_ = obj.getVal();

    return *this;
}

void BitBoard::getSetBitIndices(uint16_t* arr, uint16_t& size) const
{
    // insert the index of all the set bit into vec
    BitBoard temp(getVal());
    uint16_t itr{};
    while(temp.getVal() && itr<size){
        size_t lsbIndex = temp.getLSBIndex();
        arr[itr++] = lsbIndex;
        // erase the bit at least significant index
        temp.popBit(lsbIndex);
    }
}
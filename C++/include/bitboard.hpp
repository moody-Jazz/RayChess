#pragma once

#include <cstdint>
#include <vector>

class BitBoard
{
public:
    // Constructors
    BitBoard();                            // Default constructor
    BitBoard(uint64_t val);                // Constructor with initial value

    // Utility Functions
    uint64_t getVal() const;               // Returns the current value of val_
    void printBinary() const;              // Prints val_ in binary representation
    bool getBit(size_t square) const;      // Returns the bit state at the given index (0 to 63)
    size_t getSetBitCount() const;         // Returns the number of set bits in val_
    int getLSBIndex() const;               // Returns the rightmoset set bit index if theres none return -1

    // Mutator Functions
    void setVal(uint64_t val);             // Assigns a new value to val_
    void setBit(size_t square);            // Sets the bit state at the given index (0 to 63)
    void popBit(size_t square);            // Resets the bit at the given index (0 to 63)

    std::vector<size_t> getSetBitIndices() const; // Returns indices of all bits set to 1 in val_

private:
    uint64_t val_;                         // Internal value of the bitboard
};
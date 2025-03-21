#pragma once
#include <vector>
#include <cstdint>

// this function gives all the optimal possible blocker permutatoin at a given square of rook or bishop
std::vector<uint64_t> getBlockerPermutation(uint16_t square, char type); 

void validateMagicNumbers(char type); // this method is useful to check if a magic number combined with shiftnumber gives unique index
void initRookBishopLookupTable(); // this function is used to initialize the rook and bishop lookup table at runtime

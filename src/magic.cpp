#include "magic.hpp"
#include "precomputed.hpp"
#include "piece.hpp"
#include <unordered_set>
#include <iostream>

std::vector<uint64_t> getBlockerPermutation(uint16_t square, char type)
{
    // Get the attack bitmask without any blockers
    BitBoard attackBitboard((type == 'R')? rookAttackMinusBorder[square]: bishopAttackMinusBorder[square]);
    uint16_t setBitCount = attackBitboard.getSetBitCount();
    uint16_t totalPermutations = (1ULL << setBitCount);
    
    // get the setbit index array for the attack bitmask
    uint16_t attackBitboardIndices[setBitCount];
    attackBitboard.getSetBitIndices(attackBitboardIndices, setBitCount);

    std::vector<uint64_t> blockers(totalPermutations);
    // for all permutation of blocker bitboards that can exist map thier set bits according to the attack bitmask 
    for(uint16_t itr{}; itr < totalPermutations; itr++)
    {
        uint64_t blockerBitboard{};
        BitBoard currPermutation(itr);
        uint16_t currPermutationSize = currPermutation.getSetBitCount();
        uint16_t currPermutationSetBitIndices[currPermutationSize];
        currPermutation.getSetBitIndices(currPermutationSetBitIndices, currPermutationSize);
        
        for(uint16_t i{}; i<currPermutationSize; i++) // for every set bit put it on appropriate place according to attack bitmask
            blockerBitboard |= (1ULL << attackBitboardIndices[currPermutationSetBitIndices[i]]);
        
        blockers[itr] = blockerBitboard;
        //rookLookup[std::to_string(square) + "_" + std::to_string(blockerBitboard)] = Piece::getRookAttacks(square, blockerBitboard);
    }
    return blockers;
}

void validateMagicNumbers(char type)
{
    std::unordered_set<uint64_t> st;
    bool flag = true;
    size_t biggestIndx{};

    for(uint16_t square{}; square < 64; square++)
    {
        std::vector<uint64_t> blockers = getBlockerPermutation(square, type);
        uint64_t maxi{};
        for(uint64_t& x: blockers){
            x *= (type == 'R')? rookMagics[square]: bishopMagics[square];
            x >>= (type == 'R')? rookShifts[square]: bishopShifts[square];

            // Detect duplicate indexes
            if (st.find(x) != st.end())
                std::cout << "Duplicate index found for square " << square << ": " << x << "\n";
    
            st.insert(x);
            //std::cout<<x<<", ";
            maxi = std::max(maxi, x);
        }
        biggestIndx = std::max(biggestIndx, maxi);
        if(blockers.size() != st.size()) flag = false;
        // printf("magic number for square: %d , is %s \n",square ,(blockers.size() == st.size())?"good":"not good");
        // std::cout<<"max magic index for square: "<<square<<" is "<<maxi<<"\n";
        st.clear();
    }
    std::cout<<"\nBiggest index value is: "<<biggestIndx<<"\n";
    if(flag) std::cout<<"All the magic numbers and shift numbers are correct\n";
    else std::cout<<"Some magic number or shift numbers are not correct\n";
}

void initRookBishopLookupTable()
{
    uint64_t index{};
    for(uint16_t square{}; square < 64; square++)
    {
        std::vector<uint64_t> rookBlockerPermutation = getBlockerPermutation(square, 'R');
        std::vector<uint64_t> bishopBlockerPermutation = getBlockerPermutation(square, 'B');
        for(uint64_t& blocker: rookBlockerPermutation)
        {
            index = blocker * rookMagics[square];
            index >>= rookShifts[square];
            rookLookup[square][index] = Piece::getRookAttacks(square, blocker);
        }
        for(uint64_t& blocker: bishopBlockerPermutation)
        {
            index = blocker * bishopMagics[square];
            index >>= bishopShifts[square];
            bishopLookup[square][index] = Piece::getBishopAttacks(square, blocker);
        }
    }
}


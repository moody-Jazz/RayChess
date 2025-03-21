#pragma once
#include "cstdint"

extern uint64_t rookLookup[64][4096];
extern uint64_t bishopLookup[64][512];

extern const uint64_t pawnAttackBitmask[2][56];
extern const uint64_t pawnPushBitmask[2][56];
extern const uint64_t knightAttackBitmask[64];
extern const uint64_t kingAttackBitmask[64];
extern const uint64_t rookAttackBitmask[64];
extern const uint64_t rookAttackMinusBorder[64];
extern const uint64_t bishopAttackBitmask[64];
extern const uint64_t bishopAttackMinusBorder[64];
extern const uint64_t rookMagics[64];
extern const uint64_t bishopMagics[64];
extern const uint16_t rookShifts[64];
extern const uint16_t bishopShifts[64];
#ifndef POKEHEARTGOLD_UNK_02026DE0_H
#define POKEHEARTGOLD_UNK_02026DE0_H

#include "global.h"

typedef struct {
    const u16 *thresholds;
    const u8 *valuesLo;
    const u8 *valuesHi;
    u32 count;
} UnkStruct_02026DE0;

u16 sub_02026DE0(const UnkStruct_02026DE0 *data, u16 value);
void sub_02026E18(const void *src, UnkStruct_02026DE0 *dest);

#endif // POKEHEARTGOLD_UNK_02026DE0_H

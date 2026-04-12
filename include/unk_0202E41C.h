#ifndef POKEHEARTGOLD_UNK_0202E41C_H
#define POKEHEARTGOLD_UNK_0202E41C_H

#include "save.h"

typedef struct UnkStruct_0202E41C {
    u8 unk_000[0x0C];
    u8 unk_00C[0xC0];
    u8 unk_0CC[0xC0];
    u8 unk_18C[0xC0];
    u8 filler_24C[0x97C];
} UnkStruct_0202E41C;

u32 sub_0202E41C(void);
void sub_0202E424(UnkStruct_0202E41C *data);
void sub_0202E43C(UnkStruct_0202E41C *data);
void sub_0202E474(UnkStruct_0202E41C *data);
void sub_0202E48C(UnkStruct_0202E41C *data, u8 value);
void sub_0202E498(UnkStruct_0202E41C *data);

#endif // POKEHEARTGOLD_UNK_0202E41C_H

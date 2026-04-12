#ifndef POKEHEARTGOLD_UNK_0202DB34_H
#define POKEHEARTGOLD_UNK_0202DB34_H

#include "save.h"

typedef struct UnkStruct_0202DB34 {
    u8 unk_000[0xEC];
    u16 unk_EC;
    u16 unk_EE;
    u32 unk_F0;
    u32 unk_F4;
} UnkStruct_0202DB34;

UnkStruct_0202DB34 *sub_0202DB34(SaveData *saveData);
u32 sub_0202DB40(void);
void sub_0202DB44(UnkStruct_0202DB34 *data);
u16 sub_0202DB54(UnkStruct_0202DB34 *data);
void sub_0202DB5C(UnkStruct_0202DB34 *data, u16 value);
void sub_0202DB64(const UnkStruct_0202DB34 *src, void *dest);
void sub_0202DB70(void *dest, const UnkStruct_0202DB34 *src);
u32 sub_0202DB80(UnkStruct_0202DB34 *data);
void sub_0202DB88(UnkStruct_0202DB34 *data, u32 value);
u32 sub_0202DB90(UnkStruct_0202DB34 *data);
void sub_0202DB98(UnkStruct_0202DB34 *data, u32 value);
u16 sub_0202DBA0(UnkStruct_0202DB34 *data);

#endif // POKEHEARTGOLD_UNK_0202DB34_H

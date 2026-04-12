#include "unk_0202DB34.h"

#include <nitro/mi/memory.h>

#include "global.h"

#include "constants/save_arrays.h"
#include "save.h"

UnkStruct_0202DB34 *sub_0202DB34(SaveData *saveData) {
    return SaveArray_Get(saveData, SAVE_UNK_22);
}

u32 sub_0202DB40(void) {
    return sizeof(UnkStruct_0202DB34);
}

void sub_0202DB44(UnkStruct_0202DB34 *data) {
    MIi_CpuClearFast(0, (u32 *)data, sizeof(UnkStruct_0202DB34));
}

u16 sub_0202DB54(UnkStruct_0202DB34 *data) {
    return data->unk_EC;
}

void sub_0202DB5C(UnkStruct_0202DB34 *data, u16 value) {
    data->unk_EC = value;
}

void sub_0202DB64(const UnkStruct_0202DB34 *src, void *dest) {
    MIi_CpuCopyFast((const u32 *)src, (u32 *)dest, 0xEC);
}

void sub_0202DB70(void *dest, const UnkStruct_0202DB34 *src) {
    MIi_CpuCopyFast((const u32 *)src, (u32 *)dest, 0xEC);
}

u32 sub_0202DB80(UnkStruct_0202DB34 *data) {
    return data->unk_F0;
}

void sub_0202DB88(UnkStruct_0202DB34 *data, u32 value) {
    data->unk_F0 = value;
}

u32 sub_0202DB90(UnkStruct_0202DB34 *data) {
    return data->unk_F4;
}

void sub_0202DB98(UnkStruct_0202DB34 *data, u32 value) {
    data->unk_F4 = value;
}

u16 sub_0202DBA0(UnkStruct_0202DB34 *data) {
    return data->unk_EE;
}

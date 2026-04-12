#include "unk_0202E41C.h"

#include <nitro/mi/memory.h>

#include "global.h"

#include "save.h"

u32 sub_0202E41C(void) {
    return sizeof(UnkStruct_0202E41C);
}

void sub_0202E424(UnkStruct_0202E41C *data) {
    MIi_CpuClearFast(0, (u32 *)data, sizeof(UnkStruct_0202E41C));
    SaveSubstruct_UpdateCRC(SAVE_UNK_23);
}

void sub_0202E43C(UnkStruct_0202E41C *data) {
    MIi_CpuClearFast(0, (u32 *)data->unk_00C, 0xC0);
    MIi_CpuClearFast(0, (u32 *)data->unk_0CC, 0xC0);
    MIi_CpuClearFast(0, (u32 *)data->unk_18C, 0xC0);
    sub_0202E474(data);
    SaveSubstruct_UpdateCRC(SAVE_UNK_23);
}

void sub_0202E474(UnkStruct_0202E41C *data) {
    sub_0202E48C(data, 0);
    sub_0202E498(data);
    SaveSubstruct_UpdateCRC(SAVE_UNK_23);
}

void sub_0202E48C(UnkStruct_0202E41C *data, u8 value) {
    data->unk_000[8] = value;
    SaveSubstruct_UpdateCRC(SAVE_UNK_23);
}

void sub_0202E498(UnkStruct_0202E41C *data) {
    int i;

    for (i = 0; i < 4; i++) {
        data->unk_000[i] = 0;
    }
    SaveSubstruct_UpdateCRC(SAVE_UNK_23);
}

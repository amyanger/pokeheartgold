#include "unk_020318C8.h"

#include <nitro/mi/memory.h>

#include "global.h"

#include "constants/save_arrays.h"
#include "save.h"

u32 sub_020318C8(void) {
    return sizeof(UnkStruct_020318C8);
}

void sub_020318CC(UnkStruct_020318C8 *data) {
    MI_CpuClear32(data, sub_020318C8());
    data->unk_0 = -1;
}

UnkStruct_020318C8 *sub_020318E8(SaveData *saveData) {
    return SaveArray_Get(saveData, SAVE_UNK_32);
}

u32 sub_020318F4(UnkStruct_020318C8 *data) {
    return data->unk_0;
}

u32 sub_020318F8(UnkStruct_020318C8 *data) {
    return data->unk_4;
}

void sub_020318FC(UnkStruct_020318C8 *data, u32 value) {
    data->unk_0 = value;
}

void sub_02031900(UnkStruct_020318C8 *data, u32 value) {
    data->unk_4 = value;
}

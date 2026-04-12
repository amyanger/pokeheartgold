#include "unk_02026DE0.h"

#include "global.h"

u16 sub_02026DE0(const UnkStruct_02026DE0 *data, u16 value) {
    u32 i = 0;
    u32 count = data->count - 1;

    if (count != 0) {
        const u16 *thresholds = data->thresholds;
        do {
            if (thresholds[1] > value) {
                break;
            }
            i++;
            thresholds++;
        } while (i < count);
    }
    u8 lo = data->valuesLo[i];
    u8 hi = data->valuesHi[i];
    return lo | (hi << 8);
}

void sub_02026E18(const void *src, UnkStruct_02026DE0 *dest) {
    const u32 *data = src;

    dest->count = data[0];
    dest->thresholds = (const u16 *)&data[1];
    dest->valuesLo = (const u8 *)dest->thresholds + dest->count * 2;
    dest->valuesHi = dest->valuesLo + dest->count;
}

#include "unk_02092BB8.h"

#include "global.h"

#include "heap.h"

struct UnkStruct_02092BB8 *sub_02092BB8(enum HeapID heapID) {
    struct UnkStruct_02092BB8 *data = Heap_Alloc(heapID, sizeof(struct UnkStruct_02092BB8));
    sub_02092BD8(data, 0, 2);
    return data;
}

void sub_02092BD0(struct UnkStruct_02092BB8 *data) {
    Heap_Free(data);
}

void sub_02092BD8(struct UnkStruct_02092BB8 *data, u16 species, u16 sortMode) {
    data->unk_0 = species;
    data->unk_2 = sortMode;
}

u16 sub_02092BE0(struct UnkStruct_02092BB8 *data) {
    return data->unk_2;
}

u32 sub_02092BE4(struct UnkStruct_02092BB8 *data) {
    return data->unk_0;
}

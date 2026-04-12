#include "unk_02078DD8.h"

#include "global.h"

#include "assert.h"
#include "heap.h"
#include "player_data.h"

extern BOOL ov00_021E6D60(void *, s32);

void *sub_02078DD8(PlayerProfile *profile, enum HeapID heapID) {
    u8 *data = Heap_Alloc(heapID, 0x24);
    memset(data, 0, 0x24);
    data[0x1B] = 0x1C;
    data[0x1E] = PlayerProfile_GetTrainerGender(profile);
    data[0x1D] = PlayerProfile_GetAvatar(profile);
    data[0x18] = PlayerProfile_GetVersion(profile);
    data[0x19] = PlayerProfile_GetLanguage(profile);
    GF_ASSERT(ov00_021E6D60(data, 0x24) == 1);
    return data;
}

void sub_02078E28(void *data) {
    Heap_Free(data);
}

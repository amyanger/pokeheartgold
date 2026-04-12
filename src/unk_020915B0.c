#include "unk_020915B0.h"

#include <nitro/fs/overlay.h>
#include <nitro/os.h>

#include "global.h"

#include "constants/heap.h"
#include "heap.h"
#include "overlay_manager.h"
#include "poke_overlay.h"

FS_EXTERN_OVERLAY(OVY_0);
FS_EXTERN_OVERLAY(OVY_13);
FS_EXTERN_OVERLAY(OVY_38);

extern void ov13_0221BA00(enum HeapID heapID);

void LoadDwcOverlay(void) {
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_0), OVY_LOAD_ASYNC);
}

void UnloadDwcOverlay(void) {
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_0));
}

void LoadOVY13(void) {
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_13), OVY_LOAD_ASYNC);
}

void UnloadOVY13(void) {
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_13));
}

void sub_020915F0(void *unused, enum HeapID heapID) {
    LoadDwcOverlay();
    LoadOVY13();
    ov13_0221BA00(heapID);
    UnloadOVY13();
    UnloadDwcOverlay();
    OS_ResetSystem(0);
}

void LoadOVY38(void) {
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_38), OVY_LOAD_ASYNC);
}

void UnloadOVY38(void) {
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_38));
}

static BOOL sub_02091634(OverlayManager *man, int *state) {
    Heap_Create(HEAP_ID_3, HEAP_ID_48, 0x41000);
    sub_020915F0(((void **)OverlayManager_GetArgs(man))[2], HEAP_ID_48);
    Heap_Destroy(HEAP_ID_48);
    OS_ResetSystem(0);
    return TRUE;
}

const OverlayManagerTemplate gApp_MainMenu_SelectOption_NintendoWFCSetup = {
    sub_02091634,
    NULL,
    NULL,
    FS_OVERLAY_ID_NONE,
};

#include "unk_02097B78.h"

#include <nitro/mi/memory.h>

#include "global.h"

#include "heap.h"
#include "constants/heap.h"
#include "overlay_manager.h"

typedef BOOL (*LegendaryCinematicFunc)(LegendaryCinematicArgs *args);

extern BOOL ScriptCinematic_HoOh(LegendaryCinematicArgs *args);
extern BOOL ScriptCinematic_Lugia(LegendaryCinematicArgs *args);
extern BOOL ScriptCinematic_Arceus(LegendaryCinematicArgs *args);

static const LegendaryCinematicFunc sLegendaryCinematicFuncs[] = {
    ScriptCinematic_HoOh,
    ScriptCinematic_Lugia,
    ScriptCinematic_Arceus,
};

typedef struct LegendaryCinematicData {
    LegendaryCinematicArgs *args;
    u8 unk[0x418];
} LegendaryCinematicData;

BOOL LegendaryCinematic_Init(OverlayManager *man, int *state) {
    Heap_Create(HEAP_ID_3, HEAP_ID_153, 2 << 18);
    LegendaryCinematicData *data = OverlayManager_CreateAndGetData(man, 0x41C, HEAP_ID_153);
    MI_CpuFill8(data, 0, 0x41C);
    data->args = OverlayManager_GetArgs(man);
    return TRUE;
}

BOOL LegendaryCinematic_Main(OverlayManager *man, int *state) {
    LegendaryCinematicData *data = OverlayManager_GetData(man);
    if (sLegendaryCinematicFuncs[data->args->unk4C]((LegendaryCinematicArgs *)data) == 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL LegendaryCinematic_Exit(OverlayManager *man, int *state) {
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_153);
    return TRUE;
}

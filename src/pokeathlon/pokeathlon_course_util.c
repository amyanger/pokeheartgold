#include "nitro/mi/memory.h"

#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "assert.h"
#include "filesystem.h"
#include "player_data.h"
#include "system.h"

extern BOOL ov96_021E8828(void *system);
extern const u8 ov96_0221A934[];
extern void ov96_021E75BC(void *data);
extern u16 ov96_021E679C(u16 a, u16 b);

// ov96_021E5E04 — Initialize participant order array from external data, run pattern match
void ov96_021E5E04(PokeathlonCourseData *data, const u8 *participantOrder) {
    int i;

    for (i = 0; i < data->maxParticipants; i++) {
        data->field_3D8[i] = participantOrder[i];
    }

    data->field_3E8 = (void *)ov96_021E5E7C(data);
    data->field_27C = data->field_3E8;
}

// ov96_021E5E44 — Get participant ID at current index
u32 ov96_021E5E44(PokeathlonCourseData *data) {
    return data->field_3D8[data->field_1F0];
}

// ov96_021E5E58 — Get participant ID at index with bounds check
u32 ov96_021E5E58(PokeathlonCourseData *data, u32 index) {
    if (index >= data->maxParticipants) {
        GF_ASSERT(FALSE);
        return 0;
    }
    return data->field_3D8[index];
}

// ov96_021E5E7C — Pattern match participant IDs against table.
// NONMATCHING: C version can't reproduce the exact loop codegen; keep the
// original asm bytes via an inline-asm fallback for matching builds.
#ifdef NONMATCHING
u32 ov96_021E5E7C(PokeathlonCourseData *data) {
    BOOL isSpecial;
    u32 *ids;
    u8 count;
    u8 i, j;
    BOOL match;

    if (ov96_021E5EE8(data) == 1) {
        isSpecial = TRUE;
    } else {
        isSpecial = FALSE;
    }

    ids = data->field_3D8;

    if (isSpecial) {
        count = 4;
    } else {
        count = 3;
    }

    for (i = 0; i < 10; i++) {
        match = TRUE;
        for (j = 0; j < count; j++) {
            if (ids[j] != ov96_0221A934[i * 4 + j]) {
                match = FALSE;
                break;
            }
        }
        if (match) {
            return i;
        }
    }

    return 10;
}
#else
// clang-format off
asm u32 ov96_021E5E7C(PokeathlonCourseData *data) {
    push {r3, r4, r5, r6, r7, lr}
    add r4, r0, #0
    bl ov96_021E5EE8
    cmp r0, #1
    bne _021E5E8C
    mov r1, #1
    b _021E5E8E
_021E5E8C:
    mov r1, #0
_021E5E8E:
    mov r0, #0xf6
    lsl r0, r0, #2
    add r3, r4, r0
    cmp r1, #0
    beq _021E5E9C
    mov r0, #4
    b _021E5E9E
_021E5E9C:
    mov r0, #3
_021E5E9E:
    lsl r0, r0, #0x18
    lsr r2, r0, #0x18
    mov r0, #0
_021E5EA4:
    mov r4, #1
    mov r1, #0
    cmp r2, #0
    bls _021E5ECA
    ldr r5, =ov96_0221A934
    lsl r6, r0, #2
    add r5, r5, r6
_021E5EB2:
    lsl r6, r1, #2
    ldr r7, [r3, r6]
    ldrb r6, [r5, r1]
    cmp r7, r6
    beq _021E5EC0
    mov r4, #0
    b _021E5ECA
_021E5EC0:
    add r1, r1, #1
    lsl r1, r1, #0x18
    lsr r1, r1, #0x18
    cmp r1, r2
    blo _021E5EB2
_021E5ECA:
    cmp r4, #0
    bne _021E5EDA
    add r0, r0, #1
    lsl r0, r0, #0x18
    lsr r0, r0, #0x18
    cmp r0, #0xa
    blo _021E5EA4
    mov r0, #0xa
_021E5EDA:
    pop {r3, r4, r5, r6, r7, pc}
}
// clang-format on
#endif

// ov96_021E5EE0 — Get field_1F0
u32 ov96_021E5EE0(PokeathlonCourseData *data) {
    return data->field_1F0;
}

// ov96_021E5EE8 — Get args mode (full u32)
u32 ov96_021E5EE8(PokeathlonCourseData *data) {
    return data->args->mode;
}

// ov96_021E5EF4 — Get field_1EF
u8 ov96_021E5EF4(PokeathlonCourseData *data) {
    return data->field_1EF;
}

// ov96_021E5F00 — Increment field_1EF
void ov96_021E5F00(PokeathlonCourseData *data) {
    data->field_1EF++;
}

// ov96_021E5F10 — Zero field_1EF
void ov96_021E5F10(PokeathlonCourseData *data) {
    data->field_1EF = 0;
}

// ov96_021E5F1C — Get system pointer
void *ov96_021E5F1C(PokeathlonCourseData *data) {
    return data->system;
}

// ov96_021E5F24 — Tail call to ov96_021E8828 with system pointer
BOOL ov96_021E5F24(PokeathlonCourseData *data) {
    return ov96_021E8828(data->system);
}

// ov96_021E5F34 — Get player profile (tail call)
PlayerProfile *ov96_021E5F34(PokeathlonCourseData *data, int index) {
    return PokeathlonCourse_GetPlayerProfile(data->playerProfiles, index);
}

// ov96_021E5F44 — Calculate participant extended data address
void *ov96_021E5F44(PokeathlonCourseData *data, int index) {
    return (u8 *)data + 0x974 + index * 0x74;
}

// ov96_021E5F54 — Get dataCopySource pointer
u8 *ov96_021E5F54(PokeathlonCourseData *data) {
    return data->dataCopySource;
}

// ov96_021E5F5C — Clear dataCopySource area
void ov96_021E5F5C(PokeathlonCourseData *data) {
    MI_CpuFill8(data->dataCopySource, 0, 0x128);
}

// ov96_021E5F70 — Initialize state transition fields
void ov96_021E5F70(PokeathlonCourseData *data, u32 a1, u32 a2, u32 a3) {
    data->field_3A4 = a1;
    data->field_3A8 = a2;
    data->field_3AC = a3;
    data->field_3B0 = 1;
}

// ov96_021E5F8C — Clear state transition fields
void ov96_021E5F8C(PokeathlonCourseData *data) {
    data->field_3A4 = 0;
    data->field_3A8 = 0;
    data->field_3AC = 0;
    data->field_3B0 = 0;
}

// ov96_021E5FA4 — Get field_3A8
u32 ov96_021E5FA4(PokeathlonCourseData *data) {
    return data->field_3A8;
}

// ov96_021E5FAC — Return constant 4
u32 ov96_021E5FAC(void) {
    return 4;
}

// ov96_021E5FB0 — Write u16 into area at 0x5E0 with stride 4
void ov96_021E5FB0(PokeathlonCourseData *data, int index, u16 value) {
    *(u16 *)((u8 *)data + index * 4 + 0x5E0) = value;
}

// ov96_021E5FBC — Read u16 from area at 0x5F0 with stride 4
u16 ov96_021E5FBC(PokeathlonCourseData *data, int index) {
    return *(u16 *)((u8 *)data + index * 4 + 0x5F0);
}

// ov96_021E5FC8 — Set state transition with exit-flag assertion
void ov96_021E5FC8(PokeathlonCourseData *data, u8 value) {
    if (data->courseState.exitFlag == 1) {
        GF_ASSERT(FALSE);
    }
    data->courseState.exitFlag = 1;
    ((PokeathlonStateInfo *)data->courseState.argsPtr)->field_07 = value;
}

// ov96_021E5FEC — Conditional state transition
void ov96_021E5FEC(PokeathlonCourseData *data, u8 value, u8 compare) {
    if (((PokeathlonStateInfo *)data->courseState.argsPtr)->field_07 == compare) {
        return;
    }
    if (data->courseState.exitFlag == 1) {
        GF_ASSERT(FALSE);
    }
    data->courseState.exitFlag = 1;
    ((PokeathlonStateInfo *)data->courseState.argsPtr)->field_07 = value;
}

// ov96_021E601C — Set courseState.transitionType if args->mode == 1
void ov96_021E601C(PokeathlonCourseData *data, u32 value) {
    if (data->args->mode == 1) {
        data->courseState.transitionType = value;
    }
}

// ov96_021E6030 — Register V-blank callback
void ov96_021E6030(PokeathlonCourseData *data) {
    Main_SetVBlankIntrCB(ov96_021E75BC, data);
}

// ov96_021E6040 — Get graphicsSystem pointer
void *ov96_021E6040(PokeathlonCourseData *data) {
    return data->graphicsSystem;
}

// ov96_021E604C — Load course data from NARC archive
void ov96_021E604C(PokeathlonCourseData *data) {
    NARC *narc;
    PokeathlonCourseData *currentParticipant;
    int i;
    u8 *currentDest;
    int j;

    narc = NARC_New((NarcId)0xa9, data->heapId);
    currentParticipant = data;
    i = 0;
    currentDest = (u8 *)data + 0x618;

    for (; i < 4; i++) {
        PokeathlonCourseData *p = currentParticipant;
        u8 *d = currentDest;
        for (j = 0; j < 3; j++) {
            u16 memberId = ov96_021E679C(
                *(u16 *)((u8 *)p + 0x3F0),
                *(u16 *)((u8 *)p + 0x3F2)
            );
            NARC_ReadWholeMember(narc, memberId, d);
            p = (PokeathlonCourseData *)((u8 *)p + 0x28);
            d += 0x14;
        }
        currentParticipant = (PokeathlonCourseData *)((u8 *)currentParticipant + 0x7c);
        currentDest += 0x3c;
    }

    NARC_Delete(narc);
}

// ov96_021E60C0 — Calculate course data address (data + 0x618 + a1*0x3c + a2*0x14)
void *ov96_021E60C0(PokeathlonCourseData *data, int a1, int a2) {
    return (u8 *)data + 0x618 + a1 * 0x3c + a2 * 0x14;
}

// ov96_021E60D8 — Participant extended data with bounds checks
void *ov96_021E60D8(PokeathlonCourseData *data, int a1, int a2) {
    GF_ASSERT(a1 < 4);
    GF_ASSERT(a2 < 3);
    return (u8 *)data + 0x3F8 + a1 * 0x7c + a2 * 0x28;
}

// ov96_021E6104 — Return constant 0x50
u32 ov96_021E6104(void) {
    return 0x50;
}

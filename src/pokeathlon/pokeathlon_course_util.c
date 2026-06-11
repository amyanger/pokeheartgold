#include "nitro/mi/memory.h"

#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "assert.h"
#include "filesystem.h"
#include "follow_mon.h"
#include "player_data.h"
#include "sound_02004A44.h"
#include "sprite.h"
#include "sys_task_api.h"
#include "system.h"
#include "unk_02005D10.h"

extern BOOL ov96_021E8828(void *system);
extern const u8 ov96_0221A934[];
extern void ov96_021E75BC(void *data);
extern u16 ov96_021E679C(u16 a, u16 b);
extern void *ov96_021EA214(int a1, int a2, int a3, int a4, int heapId);
extern void *ov96_021EA4D4(int a1, int a2, int a3, int heapId);
extern void *ov96_021EA584(int a1, int a2, int a3, int heapId);
extern void *ov96_021EA6E4(int a1, int a2, int a3, int a4, int heapId);
extern void *ov96_021EA7A4(u32 a1, u32 a2, u32 a3, int heapId);
extern int ov96_021EB5E8(int a);
extern BOOL ov96_021E661C(PokeathlonCourseData *data);
extern void ov96_021E81D8(SysTask *task, void *data);

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

// ov96_021E6108 — Decode enum byte at offset 8, assert if out of range
u8 ov96_021E6108(void *p) {
    u8 val;

    if (p == NULL) {
        return 0;
    }
    val = ((u8 *)p)[8];
    switch (val) {
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
        return 3;
    }
    GF_ASSERT(FALSE);
    return 0;
}

// ov96_021E6138 — Decode enum byte at offset 6, assert if out of range
u8 ov96_021E6138(void *p) {
    u8 val;

    if (p == NULL) {
        return 0;
    }
    val = ((u8 *)p)[6];
    switch (val) {
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
        return 3;
    }
    GF_ASSERT(FALSE);
    return 0;
}

// ov96_021E6168 — Populate a 16-byte slot info struct from participant data,
// then probe NARC 0x8d for a gender/form variant flag.
void ov96_021E6168(PokeathlonCourseData *data, int partIndex, int slot, void *dest) {
    u8 buf[0x1C];
    u8 *src;
    u8 *d = (u8 *)dest;

    if (slot >= 3) {
        GF_ASSERT(FALSE);
    }

    src = PokeathlonCourse_GetParticipantUnk04(data, partIndex) + slot * 0x28;

    d[6] = src[0x10];
    *(u16 *)(d + 0) = *(u16 *)(src + 0);
    *(u16 *)(d + 2) = *(u16 *)(src + 2);
    d[7] = src[0x11];
    *(u32 *)(d + 8) = (u32)partIndex;
    *(u32 *)(d + 0xc) = *(u32 *)(src + 4);

    if (*(u16 *)d == 0) {
        GF_ASSERT(FALSE);
    }
    if (*(u16 *)d == 0) {
        *(u16 *)d = 1;
    }

    ReadWholeNarcMemberByIdPair(buf, (NarcId)0x8d, SpeciesToOverworldModelIndexOffset(*(u16 *)d));
    if (buf[1] != 0) {
        *(u16 *)(d + 4) = 1;
    } else {
        *(u16 *)(d + 4) = 0;
    }
}

// ov96_021E61D8 — Allocate three sprites and position them via Sprite_SetMatrix.
// Returns &data->field_708 (pointer to the first allocated sprite slot).
void *ov96_021E61D8(PokeathlonCourseData *data, int a1, int a2, int a3) {
    VecFx32 m2;
    VecFx32 m1;

    data->field_708 = ov96_021EA214(a2, a3, 0, 1, data->heapId);
    data->field_710 = ov96_021EA4D4(a2, a3, 0, data->heapId);
    data->field_714 = ov96_021EA584(a2, a3, 0, data->heapId);

    m1.z = 0;
    m1.x = 0x80000;
    m1.y = (a1 + 0x60) << 12;
    Sprite_SetMatrix((Sprite *)data->field_708, &m1);

    data->field_70E = (u16)a1;
    m2.z = 0;
    m2.x = 0x80000;
    m2.y = (data->field_70E + 0x48) << 12;
    Sprite_SetMatrix((Sprite *)data->field_714, &m2);

    data->field_70D = 0;
    m1.y = (a1 + 0x82) << 12;
    Sprite_SetMatrix((Sprite *)data->field_710, &m1);

    return &data->field_708;
}

// ov96_021E6290 — Wrapper that preprocesses a3 via ov96_021EB5E8 before
// calling ov96_021E61D8.
void *ov96_021E6290(PokeathlonCourseData *data, int a1, int a2, int a3) {
    return ov96_021E61D8(data, a1, a2, ov96_021EB5E8(a3));
}

// ov96_021E62AC — Create `count` sprites into field_71C[0..count), position
// each via Sprite_SetMatrix using (a2, positions[i]), and clear the
// remaining slots up to index 3. Resets field_729 at the end.
void ov96_021E62AC(PokeathlonCourseData *data, int a2, int a3, int a4, int a5, u32 count, const u16 *positions) {
    VecFx32 m;
    u8 i;

    data->field_728 = (u8)count;

    for (i = 0; i < (u8)count; i++) {
        data->field_71C[i] = ov96_021EA6E4(a3, a4, a5, 0, data->heapId);
        m.z = 0;
        m.x = ((u32)positions[i * 2]) << 12;
        m.y = ((u32)(a2 + positions[i * 2 + 1])) << 12;
        Sprite_SetMatrix((Sprite *)data->field_71C[i], &m);
    }

    for (; i < 3; i++) {
        data->field_71C[i] = NULL;
    }

    data->field_729 = 0;
}

// ov96_021E634C — Wrapper that preprocesses a4 via ov96_021EB5E8 and forwards
// to ov96_021E62AC with a5/count/positions passed through.
void ov96_021E634C(PokeathlonCourseData *data, int a2, int a3, int a4, int a5, u8 count, const u16 *positions) {
    ov96_021E62AC(data, a2, a3, ov96_021EB5E8(a4), a5, count, positions);
}

// ov96_021E637C — Three-state animation driver keyed on field_729.
// State 0: play "start" SE, reset prior sprites, advance.
// State 1: poll ov96_021E661C; on ready advance and trigger sub_020053A8.
// State 2: play frame-timed cel changes on the primary sprite, returning
// TRUE once the animation completes (field_70C > 0x3f).
BOOL ov96_021E637C(PokeathlonCourseData *data) {
    BOOL done = FALSE;
    u8 state = data->field_729;

    if (state == 0) {
        PlaySE(0x89a);
        ov96_021E65D8(data);
        data->field_729++;
    } else if (state == 1) {
        if (ov96_021E661C(data)) {
            data->field_729++;
            sub_020053A8(7, 0);
        }
    } else if (state == 2) {
        u8 frame = data->field_70C;
        if (frame == 0) {
            PlaySE(0x89d);
            Sprite_SetDrawFlag((Sprite *)data->field_708, TRUE);
            Sprite_SetAnimCtrlSeq((Sprite *)data->field_708, 0);
        } else if (frame == 0x15) {
            PlaySE(0x89d);
            Sprite_SetAnimCtrlSeq((Sprite *)data->field_708, 1);
        } else if (frame == 0x2a) {
            PlaySE(0x89d);
            Sprite_SetAnimCtrlSeq((Sprite *)data->field_708, 2);
        }
        data->field_70C++;
        if (data->field_70C > 0x3f) {
            PlaySE(0x892);
            Sprite_SetDrawFlag((Sprite *)data->field_708, FALSE);
            data->field_70C = 0;
            done = TRUE;
        }
    }

    return done;
}

// ov96_021E6454 — Compute tier = (param + 0x1D) / 0x1E. For tier in [1..3],
// show the field_714 sprite on the matching anim sequence, playing a
// progression SE when the tier advances. Otherwise hide the sprite.
void ov96_021E6454(PokeathlonCourseData *data, int param) {
    int tier = (param + 0x1d) / 0x1e;

    if (tier <= 0) {
        Sprite_SetDrawFlag((Sprite *)data->field_714, FALSE);
        return;
    }
    if (tier > 3) {
        Sprite_SetDrawFlag((Sprite *)data->field_714, FALSE);
        return;
    }
    if (data->field_72B != tier) {
        PlaySE(0x897);
        data->field_72B = (u8)tier;
    }
    Sprite_SetDrawFlag((Sprite *)data->field_714, TRUE);
    Sprite_SetAnimCtrlSeq((Sprite *)data->field_714, tier + 1);
}

// ov96_021E64B8 — Initialize the sprite task table: zero count, assert each
// slot is already inactive, clear arg/sprite/task fields.
void ov96_021E64B8(PokeathlonCourseData *data) {
    u8 i;

    data->field_D30 = 0;
    for (i = 0; i < 3; i++) {
        if (data->spriteTasks[i].active != 0) {
            GF_ASSERT(FALSE);
        }
        data->spriteTasks[i].arg = 0;
        data->spriteTasks[i].sprite = NULL;
        data->spriteTasks[i].task = NULL;
    }
}

// ov96_021E64F8 — Allocate a sprite task slot, create the sprite via
// ov96_021EA7A4, register a SysTask driver, and return the sprite pointer.
void *ov96_021E64F8(PokeathlonCourseData *data, u32 a1, u32 a2, u32 a3, u32 a4) {
    PokeathlonSpriteTask *slot;

    if (data->field_D30 >= 3) {
        GF_ASSERT(FALSE);
        return NULL;
    }

    slot = &data->spriteTasks[data->field_D30];
    slot->active = 1;
    slot->arg = a1;
    slot->sprite = ov96_021EA7A4(a2, a3, a4, data->heapId);
    slot->task = SysTask_CreateOnMainQueue(ov96_021E81D8, slot, 1);
    data->field_D30++;
    return slot->sprite;
}

// ov96_021E6550 — Tear down all sprite task slots: zero active/arg/sprite,
// destroy each SysTask, then clear its task pointer.
void ov96_021E6550(PokeathlonCourseData *data) {
    u8 i;

    for (i = 0; i < 3; i++) {
        data->spriteTasks[i].active = 0;
        data->spriteTasks[i].arg = 0;
        data->spriteTasks[i].sprite = NULL;
        SysTask_Destroy((SysTask *)data->spriteTasks[i].task);
        data->spriteTasks[i].task = NULL;
    }
}

// ov96_021E658C — Tail-call Sprite_TryChangeAnimSeq on a task slot's sprite.
void ov96_021E658C(PokeathlonCourseData *data, int index, int seq) {
    Sprite_TryChangeAnimSeq((Sprite *)data->spriteTasks[index].task, seq);
}

// ov96_021E65A4 — Halt all sprite task animations; assert each slot is live.
void ov96_021E65A4(PokeathlonCourseData *data) {
    u8 i;

    for (i = 0; i < 3; i++) {
        if (data->spriteTasks[i].active == 0) {
            GF_ASSERT(FALSE);
        }
        Sprite_SetAnimActiveFlag((Sprite *)data->spriteTasks[i].task, FALSE);
    }
}

// ov96_021E65D8 — Show and reset animation sequences for the first field_728
// sprite slots in field_71C.
void ov96_021E65D8(PokeathlonCourseData *data) {
    u8 i;

    if (data->field_728 == 0) {
        return;
    }
    for (i = 0; i < data->field_728; i++) {
        Sprite_SetDrawFlag((Sprite *)data->field_71C[i], TRUE);
        Sprite_SetAnimCtrlSeq((Sprite *)data->field_71C[i], 0);
    }
}

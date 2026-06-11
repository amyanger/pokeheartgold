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

extern u16 ov96_021E679C(u16 a, u16 b);
extern void *ov96_021EA214(int a1, int a2, int a3, int a4, int heapId);
extern void *ov96_021EA4D4(int a1, int a2, int a3, int heapId);
extern void *ov96_021EA584(int a1, int a2, int a3, int heapId);
extern void *ov96_021EA6E4(int a1, int a2, int a3, int a4, int heapId);
extern void *ov96_021EA7A4(u32 a1, u32 a2, u32 a3, int heapId);
extern int ov96_021EB5E8(int a);
extern BOOL ov96_021E661C(PokeathlonCourseData *data);
extern void ov96_021E81D8(SysTask *task, void *data);

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
    u8 buf[4];
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
    VecFx32 m1;
    VecFx32 m2;

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
// NONMATCHING: mwcc register-caches one of the stack args (count/positions),
// while the original reloads all three from their home slots on every use.
// Keep the original bytes via an inline-asm fallback for matching builds.
#ifdef NONMATCHING
void ov96_021E62AC(PokeathlonCourseData *data, int a2, int a3, int a4, int a5, u32 count, const u16 *positions) {
    VecFx32 m;
    u8 i;

    data->field_728 = count;

    for (i = 0; i < count; i++) {
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
#else
// clang-format off
asm void ov96_021E62AC(PokeathlonCourseData *data, int a2, int a3, int a4, int a5, u32 count, const u16 *positions) {
    push {r4, r5, r6, r7, lr}
    sub sp, #0x1c
    add r7, r0, #0
    ldr r0, [sp, #0x30]
    str r1, [sp, #4]
    str r0, [sp, #0x30]
    ldr r0, [sp, #0x34]
    ldr r1, =0x00000728
    str r0, [sp, #0x34]
    ldr r0, [sp, #0x38]
    str r2, [sp, #8]
    str r0, [sp, #0x38]
    ldr r0, [sp, #0x34]
    str r3, [sp, #0xc]
    strb r0, [r7, r1]
    ldr r0, [sp, #0x34]
    mov r4, #0
    cmp r0, #0
    bls _021E631E
_021E62D2:
    mov r0, #0xa1
    lsl r0, r0, #2
    ldr r0, [r7, r0]
    lsl r6, r4, #2
    str r0, [sp]
    ldr r0, [sp, #8]
    ldr r1, [sp, #0xc]
    ldr r2, [sp, #0x30]
    mov r3, #0
    add r5, r7, r6
    bl ov96_021EA6E4
    ldr r1, =0x0000071C
    str r0, [r5, r1]
    mov r0, #0
    str r0, [sp, #0x18]
    ldr r1, [sp, #0x38]
    ldr r0, [sp, #0x38]
    ldrh r1, [r1, r6]
    add r0, r0, r6
    lsl r1, r1, #0xc
    str r1, [sp, #0x10]
    ldrh r1, [r0, #2]
    ldr r0, [sp, #4]
    add r0, r0, r1
    lsl r0, r0, #0xc
    str r0, [sp, #0x14]
    ldr r0, =0x0000071C
    add r1, sp, #0x10
    ldr r0, [r5, r0]
    bl Sprite_SetMatrix
    add r0, r4, #1
    lsl r0, r0, #0x18
    lsr r4, r0, #0x18
    ldr r0, [sp, #0x34]
    cmp r4, r0
    blo _021E62D2
_021E631E:
    cmp r4, #3
    bhs _021E6336
    ldr r0, =0x0000071C
    mov r2, #0
_021E6326:
    lsl r1, r4, #2
    add r1, r7, r1
    str r2, [r1, r0]
    add r1, r4, #1
    lsl r1, r1, #0x18
    lsr r4, r1, #0x18
    cmp r4, #3
    blo _021E6326
_021E6336:
    ldr r0, =0x00000729
    mov r1, #0
    strb r1, [r7, r0]
    add sp, #0x1c
    pop {r4, r5, r6, r7, pc}
}
// clang-format on
#endif

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

    switch (data->field_729) {
    case 0:
        PlaySE(0x89a);
        ov96_021E65D8(data);
        data->field_729++;
        break;
    case 1:
        if (ov96_021E661C(data)) {
            data->field_729++;
            sub_020053A8(7, 0);
        }
        break;
    case 2: {
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
        break;
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
    if (tier <= 3) {
        if (data->field_72B != tier) {
            PlaySE(0x897);
            data->field_72B = (u8)tier;
        }
        Sprite_SetDrawFlag((Sprite *)data->field_714, TRUE);
        Sprite_SetAnimCtrlSeq((Sprite *)data->field_714, tier + 1);
        return;
    }
    Sprite_SetDrawFlag((Sprite *)data->field_714, FALSE);
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

    slot = &data->spriteTasks[(u8)data->field_D30];
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
    Sprite_TryChangeAnimSeq((Sprite *)data->spriteTasks[index].sprite, seq);
}

// ov96_021E65A4 — Halt all sprite task animations; assert each slot is live.
void ov96_021E65A4(PokeathlonCourseData *data) {
    u8 i;

    for (i = 0; i < 3; i++) {
        if (data->spriteTasks[i].active == 0) {
            GF_ASSERT(FALSE);
        }
        Sprite_SetAnimActiveFlag((Sprite *)data->spriteTasks[i].sprite, FALSE);
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

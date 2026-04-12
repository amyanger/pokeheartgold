#include "unk_0202C730.h"

#include <nitro/mi/memory.h>

#include "global.h"

#include "constants/save_arrays.h"
#include "friend_group.h"
#include "math_util.h"
#include "pm_string.h"
#include "save.h"
#include "string_util.h"

static SAV_FRIEND_GRP *sSavFriendGrp;

BOOL sub_0202C8C4(FRIEND_GROUP *group);
BOOL sub_0202C8E4(FRIEND_GROUP *g1, FRIEND_GROUP *g2);

u32 Save_FriendGroup_sizeof(void) {
    return sizeof(SAV_FRIEND_GRP);
}

void sub_0202C738(SAV_FRIEND_GRP *savFriendGrp, u16 srcIdx, int dstIdx) {
    savFriendGrp->groups[dstIdx] = savFriendGrp->groups[srcIdx];
}

void Save_FriendGroup_Init(SAV_FRIEND_GRP *savFriendGrp) {
    int i;

    MIi_CpuClearFast(0, (u32 *)savFriendGrp, sizeof(SAV_FRIEND_GRP));
    for (i = 0; i < FGRP_MAX; i++) {
        savFriendGrp->groups[i].unk_0[0] = 0xFFFF;
        savFriendGrp->groups[i].unk_10[0] = 0xFFFF;
    }
    sSavFriendGrp = savFriendGrp;
}

void sub_0202C78C(SAV_FRIEND_GRP *savFriendGrp, u32 count) {
    u32 i, j;

    for (i = 0; i < FGRP_MAX; i++) {
        for (j = 0; j < count; j++) {
            savFriendGrp->groups[i].unk_28 = PRandom(savFriendGrp->groups[i].unk_28);
        }
    }
}

u32 Save_FriendGroup_GetGroupId(SAV_FRIEND_GRP *savFriendGrp, int grpIdx) {
    return savFriendGrp->groups[grpIdx].unk_24;
}

void sub_0202C7C0(SAV_FRIEND_GRP *savFriendGrp, u32 grpIdx, u32 groupId) {
    savFriendGrp->groups[grpIdx].unk_24 = groupId;
    savFriendGrp->groups[grpIdx].unk_28 = PRandom(groupId);
}

u32 sub_0202C7DC(SAV_FRIEND_GRP *savFriendGrp) {
    return savFriendGrp->groups[1].unk_28;
}

u16 *sub_0202C7E0(SAV_FRIEND_GRP *savFriendGrp, int grpIdx, int nameType) {
    if (nameType == 0) {
        return savFriendGrp->groups[grpIdx].unk_0;
    }
    return savFriendGrp->groups[grpIdx].unk_10;
}

void sub_0202C7F8(SAV_FRIEND_GRP *savFriendGrp, int grpIdx, int nameType, String *str) {
    if (nameType == 0) {
        CopyStringToU16Array(str, savFriendGrp->groups[grpIdx].unk_0, 8);
    } else {
        CopyStringToU16Array(str, savFriendGrp->groups[grpIdx].unk_10, 8);
    }
}

void sub_0202C824(SAV_FRIEND_GRP *savFriendGrp, int grpIdx, u32 value) {
    savFriendGrp->groups[grpIdx].unk_20 = value;
}

u8 sub_0202C830(SAV_FRIEND_GRP *savFriendGrp, int grpIdx) {
    return savFriendGrp->groups[grpIdx].unk_20;
}

u8 sub_0202C83C(SAV_FRIEND_GRP *savFriendGrp, int grpIdx) {
    return savFriendGrp->groups[grpIdx].unk_21;
}

void sub_0202C848(SAV_FRIEND_GRP *savFriendGrp, int grpIdx, int value) {
    savFriendGrp->groups[grpIdx].unk_21 = value;
}

SAV_FRIEND_GRP *Save_FriendGroup_Get(SaveData *saveData) {
    return SaveArray_Get(saveData, SAVE_FRIEND_GROUP);
}

BOOL sub_0202C860(SAV_FRIEND_GRP *savFriendGrp, u16 grpIdx) {
    if (sub_0202C8C4(&savFriendGrp->groups[grpIdx]) == 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL sub_0202C878(SAV_FRIEND_GRP *savFriendGrp, u16 grpIdx) {
    return sub_0202C8E4(&savFriendGrp->groups[1], &savFriendGrp->groups[grpIdx]);
}

BOOL sub_0202C88C(SAV_FRIEND_GRP *savFriendGrp, u16 *name) {
    int i;

    if (name[0] == 0xFFFF) {
        return FALSE;
    }
    for (i = 0; i < FGRP_MAX; i++) {
        if (StringNotEqualN(name, savFriendGrp->groups[i].unk_0, 8) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL sub_0202C8C4(FRIEND_GROUP *group) {
    if (group->unk_0[0] == 0xFFFF) {
        return TRUE;
    }
    if (group->unk_10[0] == 0xFFFF) {
        return TRUE;
    }
    return FALSE;
}

BOOL sub_0202C8E4(FRIEND_GROUP *g1, FRIEND_GROUP *g2) {
    if (StringNotEqualN(g1->unk_10, g2->unk_10, 8)) {
        return FALSE;
    }
    if (StringNotEqualN(g1->unk_0, g2->unk_0, 8)) {
        return FALSE;
    }
    if (g1->unk_20 != g2->unk_20) {
        return FALSE;
    }
    if (g1->unk_21 != g2->unk_21) {
        return FALSE;
    }
    if (g1->unk_24 != g2->unk_24) {
        return FALSE;
    }
    return TRUE;
}

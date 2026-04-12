#include "unk_02069660.h"

#include <nnsys/g3d/binres/res_struct_accessor.h>
#include <nnsys/g3d/binres/res_struct_accessor_inline.h>
#include <nnsys/g3d/kernel.h>
#include <nnsys/gfd.h>

#include "global.h"

#include "assert.h"
#include "gf_3d_render.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_01_021F1348.h"
#include "sys_task_api.h"

void sub_02069660(UnkOv01_021FFECC_sub *data) {
    u8 *p = (u8 *)data;
    int i;

    for (i = 0x14; i != 0; i--) {
        *p++ = 0;
    }
}

void sub_02069670(UnkOv01_021FFECC_sub *data, void *mdlData, int mdlIndex) {
    sub_02069660(data);
    data->mdlData = mdlData;
    data->loaded = 0;
    data->mdlSet = NNS_G3dGetMdlSet(mdlData);
    data->unkC = NNS_G3dGetMdlByIdx(data->mdlSet, mdlIndex);
    data->tex = NNS_G3dGetTex(mdlData);
}

void sub_020696C4(UnkOv01_021FFECC_sub *data, int mdlIndex, NARC *narc, s32 fileId, enum HeapID heapID, BOOL atEnd) {
    void *mdlData = GfGfxLoader_LoadFromOpenNarc(narc, fileId, FALSE, heapID, atEnd);
    sub_02069670(data, mdlData, mdlIndex);
}

void sub_020696E8(UnkOv01_021FFECC_sub *data) {
    GF3dRender_AllocAndLoadTexResources(data->tex);
    NNS_G3dBindMdlSet(data->mdlSet, data->tex);
    data->loaded = 1;
}

static void sub_02069700(SysTask *task, void *work) {
    sub_020696E8(work);
    SysTask_Destroy(task);
}

void sub_02069714(UnkOv01_021FFECC_sub *data) {
    if (SysTask_CreateOnVBlankQueue(sub_02069700, data, 0xFFFF) == NULL) {
        GF_ASSERT(FALSE);
    }
}

void sub_02069734(UnkOv01_021FFECC_sub *data) {
    if (data->mdlData != NULL) {
        Heap_Free(data->mdlData);
    }
}

void sub_02069744(UnkOv01_021FFECC_sub *data) {
    NNSG3dTexKey texKey;
    NNSG3dTexKey tex4x4Key;

    if (data->tex != NULL) {
        NNS_G3dTexReleaseTexKey(data->tex, &texKey, &tex4x4Key);
        (*NNS_GfdDefaultFuncFreeTexVram)(texKey);
        (*NNS_GfdDefaultFuncFreeTexVram)(tex4x4Key);
        NNS_G3dPlttReleasePlttKey(data->tex);
        (*NNS_GfdDefaultFuncFreePlttVram)(0);
        data->tex = NULL;
    }
}

void sub_02069784(UnkOv01_021FFECC_sub *data) {
    sub_02069744(data);
    sub_02069734(data);
    sub_02069660(data);
}

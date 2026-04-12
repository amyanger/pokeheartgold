#ifndef POKEHEARTGOLD_UNK_020915B0_H
#define POKEHEARTGOLD_UNK_020915B0_H

#include "global.h"
#include "heap.h"
#include "overlay_manager.h"

void LoadDwcOverlay(void);
void UnloadDwcOverlay(void);
void LoadOVY13(void);
void UnloadOVY13(void);
void sub_020915F0(void *unused, enum HeapID heapID);
void LoadOVY38(void);
void UnloadOVY38(void);

extern const OverlayManagerTemplate gApp_MainMenu_SelectOption_NintendoWFCSetup;

#endif // POKEHEARTGOLD_UNK_020915B0_H

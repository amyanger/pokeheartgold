#ifndef POKEHEARTGOLD_MAP_SECTION_H
#define POKEHEARTGOLD_MAP_SECTION_H

#include "global.h"

typedef u16 mapsec_t;

typedef enum MapsecType {
    MAPSECTYPE_NORMAL,
    MAPSECTYPE_GIFT,
    MAPSECTYPE_EXTERNAL,
    MAPSECTYPE_MAX
} MapsecType;

int sub_02017FAC(u32 mapsec);
int sub_02017FCC(u32 mapsec);
int sub_02017FE4(MapsecType type, int offset);
BOOL LocationIsDiamondPearlCompatible(mapsec_t mapsec);

#endif // POKEHEARTGOLD_MAP_SECTION_H

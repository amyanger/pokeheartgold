#include "map_section.h"

#include "assert.h"
#include "constants/map_sections.h"

static const u16 sMapsecTypeBoundaries[] = {
    MAPSEC_MYSTERY_ZONE,
    METLOC_DAY_CARE_COUPLE,
    METLOC_LOVELY_PLACE,
};

int sub_02017FAC(int mapsec) {
    int i;

    for (i = 0; i < 2; i++) {
        if ((u16)mapsec < sMapsecTypeBoundaries[i + 1]) {
            return i;
        }
    }
    return i;
}

int sub_02017FCC(int mapsec) {
    int type = sub_02017FAC(mapsec);
    return mapsec - sMapsecTypeBoundaries[type];
}

int sub_02017FE4(MapsecType type, int offset) {
    GF_ASSERT(type < MAPSECTYPE_MAX);
    return offset + sMapsecTypeBoundaries[type];
}

BOOL LocationIsDiamondPearlCompatible(mapsec_t mapsec) {
    if (mapsec >= MAPSEC_TWINLEAF_TOWN && mapsec <= MAPSEC_BATTLE_PARK) {
        return TRUE;
    }
    if (mapsec >= METLOC_DAY_CARE_COUPLE && mapsec <= METLOC_RILEY) {
        return TRUE;
    }
    if (mapsec >= METLOC_LOVELY_PLACE && mapsec <= METLOC_CONCERT_EVENT) {
        return TRUE;
    }
    return FALSE;
}

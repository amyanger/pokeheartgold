#include "unk_020971F8.h"

#include "global.h"

#include "unk_0203AC88.h"
#include "unk_02033AE0.h"

extern void ov44_02233100(void *a, void *b);
extern void ov44_02233160(void *a);
extern void ov44_0223317C(void *a, void *b);

void sub_020971F8(void *arg) {
    const UnkStruct_0203AC88 *table = sub_0203AC88();
    int count = sub_0203AC90();
    sub_0203410C((void *)table, count, arg);
}

void sub_02097214(void) {
    const UnkStruct_0203AC88 *table = sub_0203AC88();
    int count = sub_0203AC90();
    sub_0203410C((void *)table, count, NULL);
}

u32 sub_0209722C(void) {
    return 0;
}

u32 sub_02097230(void) {
    return 8;
}

u32 sub_02097234(void) {
    return 4;
}

void sub_02097238(void *a, void *b, void *c, void *d) {
    if (d != NULL) {
        ov44_02233100(d, c);
    }
}

void sub_02097248(void *a, void *b, void *c, void *d) {
    if (d != NULL) {
        ov44_02233160(d);
    }
}

void sub_02097258(void *a, void *b, void *c, void *d) {
    if (d != NULL) {
        ov44_0223317C(d, c);
    }
}

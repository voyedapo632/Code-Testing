#include <stdlib.h>
#include <stdint.h>

#pragma once
#define TYPE_NAME(t) #t
#define TYPE_ARGS(t) sizeof(t), #t
#define CHAR_INFO (item_info_new(TYPE_ARGS(char), (void *)item_info_primitive_cmp_c8, (void *)item_info_primitive_copy, NULL, NULL))
#define SHORT_INFO (item_info_new(TYPE_ARGS(short), (void *)item_info_primitive_cmp_s16, (void *)item_info_primitive_copy, NULL, NULL))
#define INT_INFO (item_info_new(TYPE_ARGS(int), (void *)item_info_primitive_cmp_i32, (void *)item_info_primitive_copy, NULL, NULL))
#define LONG_INFO (item_info_new(TYPE_ARGS(long long), (void *)item_info_primitive_cmp_l64, (void *)item_info_primitive_copy, NULL, NULL))
#define FLOAT_INFO (item_info_new(TYPE_ARGS(float), (void *)item_info_primitive_cmp_f32, (void *)item_info_primitive_copy, NULL, NULL))
#define DOUBLE_INFO (item_info_new(TYPE_ARGS(double), (void *)item_info_primitive_cmp_d64, (void *)item_info_primitive_copy, NULL, NULL))

typedef struct ItemInfoTable {
    int(*comp)(void *a, void *b);
    void(*copy)(void *item, void **dest);
    int64_t(*hash)(void *item);
    void(*free)(void ** item);
    size_t size;
    const char *name;
} ItemInfoTable;

ItemInfoTable item_info_new(size_t size, const char *name, void *comp, void *copy, void *hash, void *free);
int item_info_primitive_cmp(double *a, double *b);

ItemInfoTable item_info_new(size_t size, const char *name, void *comp, void *copy, void *hash, void *free) {
    ItemInfoTable itemInfo = {
        (int(*)(void *, void *))comp,
        (void(*)(void *, void **))copy,
        (int64_t(*)(void *))hash,
        (void(*)(void **))free,
        size,
        name
    };

    return itemInfo;
}

int item_info_primitive_cmp_c8(char *a, char *b) {
    if (*a < *b) {
        return -1;
    } else if (*a == *b) {
        return 0;
    }

    return 1;
}

int item_info_primitive_cmp_s16(short *a, short *b) {
    if (*a < *b) {
        return -1;
    } else if (*a == *b) {
        return 0;
    }

    return 1;
}

int item_info_primitive_cmp_i32(int *a, int *b) {
    if (*a < *b) {
        return -1;
    } else if (*a == *b) {
        return 0;
    }

    return 1;
}

int item_info_primitive_cmp_l64(long long *a, long long *b) {
    if (*a < *b) {
        return -1;
    } else if (*a == *b) {
        return 0;
    }

    return 1;
}

int item_info_primitive_cmp_f32(float *a, float *b) {
    if (*a < *b) {
        return -1;
    } else if (*a == *b) {
        return 0;
    }

    return 1;
}

int item_info_primitive_cmp_d64(double *a, double *b) {
    if (*a < *b) {
        return -1;
    } else if (*a == *b) {
        return 0;
    }

    return 1;
}

void item_info_primitive_copy(void *item, void **dest) {
    *dest = item;
}
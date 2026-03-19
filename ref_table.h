#include <stdlib.h>

typedef struct RefValue {
    const char *key;
    union {
        double num;
        void *ptr;
    } value;
} RefValue;

RefValue ref_num(const char *key, double value) {
    RefValue rn;

    rn.key = key;
    rn.value.num = value;
    return rn;
}

RefValue ref_ptr(const char *key, void *value) {
    RefValue rn;

    rn.key = key;
    rn.value.ptr = value;
    return rn;
}

typedef struct RefRow {
    size_t size;
    RefValue *values;
} RefRow;

typedef struct RefTable {
    size_t count;
    size_t capacity;
    RefRow *rows;
} RefTable;
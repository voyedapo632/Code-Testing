#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "list.h"

#pragma once
#ifdef __cplusplus
extern "C" {
#endif
typedef enum ValueType {
    VALUE_TYPE_ANY,
    VALUE_TYPE_NUMBER
} ValueType;

typedef struct KeyValuePair {
    ValueType type;
    const char* key;
    void* value;
} KeyValuePair;

typedef struct SafeMap {
    size_t capacity;
    size_t count;
    KeyValuePair* data;
} SafeMap;

static SafeMap* sm_new(size_t initialCapacity) {
    SafeMap* map = (SafeMap*)malloc(sizeof(SafeMap));

    if (map == NULL) {
        return NULL;
    }

    map->capacity = initialCapacity;
    map->count = 0;
    map->data = (KeyValuePair*)calloc(initialCapacity, sizeof(KeyValuePair));

    if (map->data == NULL) {
        free(map);
        return NULL;
    }

    return map;
}

static SafeMap* sm_copy(SafeMap* map) {
    SafeMap* newMap = sm_new(map->capacity);

    newMap->count = map->count;
    memcpy(newMap->data, map->data, map->capacity);
    return newMap;
}

static SafeMap* sm_resize(SafeMap* map, size_t newSize) {
    KeyValuePair* newData = (KeyValuePair* )calloc(newSize, sizeof(KeyValuePair));

    if (newData == NULL) {
        return NULL;
    }

    if (map->data != NULL) {
        memcpy(newData, map->data, map->capacity < newSize ? map->capacity : newSize);
        free(map->data);
    }

    map->data = newData;
    return map;
}

static size_t sm_key_index(const char* key) {
    if (key == NULL) {
        return 0;
    }

    size_t value = 0;

    while (*key) {
        value = value * 151 + *key++;
    }

    return value;
}

static SafeMap* sm_put(SafeMap* map, const char* key, void* value) {
    size_t offset = sm_key_index(key) % map->capacity;

    for (size_t i = offset; i != ((offset - 1) % map->capacity); i = (i + 1) % map->capacity) {
        if (map->data[i].key == NULL) {
            map->data[i].type = VALUE_TYPE_ANY;
            map->data[i].key = key;
            map->data[i].value = value;
            map->count++;
            break;
        }
    }

    return map;
}
#ifdef __cplusplus
}
#endif
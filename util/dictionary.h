#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "item_info.h"

#define DICT_INFO (item_info_new(TYPE_ARGS(Dictionary *), NULL, NULL, NULL, (void *)dict_free))

#pragma once
typedef struct Dictionary {
    size_t capacity;
    size_t size;
    const char **keys;
    void *values;
    ItemInfoTable valueInfo;
} Dictionary;

Dictionary *dict_new(size_t capacity, ItemInfoTable valueInfo);
void dict_free(Dictionary **dict);
void dict_resize(Dictionary *dict, size_t newSize);
void dict_set_key(Dictionary *dict, int64_t index, const char *key);
const char *dict_get_key(Dictionary *dict, int64_t index);
void dict_set_value(Dictionary *dict, int64_t index, void *value);
void *dict_get_value(Dictionary *dict, int64_t index);
void dict_put(Dictionary *dict, const char *key, void *value);
void *dict_get(Dictionary *dict, const char *key);
void dict_set_data(Dictionary *dict, size_t srcSize, void *src);
void dict_remove(Dictionary *dict, const char *key);

int c_str_equals(const char *s1, const char *s2) {
    while (*s1) {
        if (*s1++ != *s2++) {
            return 0;
        }
    }

    return 1;
}

int64_t c_str_hash(const char *str) {
    int64_t index = 0;

    while (*str) {
        index = index * 256 + *str++;
    }

    return index;
}

Dictionary *dict_new(size_t capacity, ItemInfoTable valueInfo) {
    Dictionary *map = (Dictionary *)malloc(sizeof(Dictionary));

    map->capacity = capacity;
    map->size = capacity / 2;
    map->keys = (const char **)calloc(capacity, sizeof(const char *));
    map->values = (void *)calloc(capacity, valueInfo.size);
    map->valueInfo = valueInfo;
    return map;
}

void dict_free(Dictionary **dict) {
    if ((*dict)->valueInfo.free != NULL) {
        for (size_t i = 0; i < (*dict)->size; i++) {
            void **item = (void **)((char *)(*dict)->values + i * (*dict)->valueInfo.size);
            (*dict)->valueInfo.free(item);
        }
    }

    free((*dict)->keys);
    free((*dict)->values);
    free(*dict);
    *dict = NULL;
}

void dict_resize(Dictionary *dict, size_t newSize) {
    if (newSize >= dict->capacity) {
        dict->capacity = newSize * 2;
        dict->keys = (const char **)realloc(dict->keys, dict->capacity * sizeof(const char *));
        memset((char *)dict->keys + dict->size * sizeof(const char *), 0, (newSize - dict->size) * sizeof(const char *));
        dict->values = realloc(dict->values, dict->capacity * dict->valueInfo.size);
    }

    dict->size = newSize;
}

void dict_set_key(Dictionary *dict, int64_t index, const char *key) {
    dict->keys[index] = key;
}

const char *dict_get_key(Dictionary *dict, int64_t index) {
    return dict->keys[index];
}

void dict_set_value(Dictionary *dict, int64_t index, void *value) {
    memcpy((char *)dict->values + index * dict->valueInfo.size, value, dict->valueInfo.size);
}

void *dict_get_value(Dictionary *dict, int64_t index) {
    return (void *)((char *)dict->values + index * dict->valueInfo.size);
}

void dict_put(Dictionary *dict, const char *key, void *value) {
    int64_t modIndex = c_str_hash(key) % dict->size;

    if (dict->keys[modIndex] == NULL || c_str_equals(dict->keys[modIndex], key)) {
        dict->keys[modIndex] = key;
        dict_set_value(dict, modIndex, value);
    } else {
        while (dict->size < dict->capacity) {
            modIndex = c_str_hash(key) % dict->size;

            if (dict->keys[modIndex] == NULL) {
                dict->keys[modIndex] = key;
                dict_set_value(dict, modIndex, value);
                return;
            }

            dict->size++;
        }

        for (int i = 0; i < dict->size; i++) {
            int64_t index = (modIndex + i) % dict->size;

            if (dict->keys[index] == NULL || c_str_equals(dict->keys[index], key)) {
                dict->keys[index] = key;
                dict_set_value(dict, index, value);
                return;
            }
        }

        dict_resize(dict, dict->capacity);
        dict->keys[dict->size - 1] = key;
        dict_set_value(dict, dict->size - 1, value);
    }
}

void *dict_get(Dictionary *dict, const char *key) {
    int64_t modIndex = c_str_hash(key) % dict->size;

    for (int i = 0; i < dict->size; i++) {
        int64_t index = (modIndex + i) % dict->size;

        if (c_str_equals(dict->keys[index], key)) {
            return dict_get_value(dict, index);
        }
    }

    return NULL;
}

void dict_set_data(Dictionary *dict, size_t srcSize, void *src) {
    size_t stride = sizeof(const char *);

    if (sizeof(const char *) >= dict->valueInfo.size) {
        stride += sizeof(const char *);
    } else {
        stride += dict->valueInfo.size + (dict->valueInfo.size - sizeof(const char *));
    }

    for (int i = 0; i < srcSize; i += stride) {
        const char *key = *(const char **)((char *)src + i);
        void *value = (void *)((char *)src + i + sizeof(const char *));
        dict_put(dict, key, value);
    }
}

void dict_remove(Dictionary *dict, const char *key) {
    int64_t modIndex = c_str_hash(key) % dict->size;

    for (int i = 0; i < dict->size; i++) {
        int64_t index = (modIndex + i) % dict->size;

        if (c_str_equals(dict->keys[index], key)) {
            dict->keys[index] = NULL;
            return;
        }
    }
}
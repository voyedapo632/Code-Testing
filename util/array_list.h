#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "item_info.h"

#define ARRAY_LIST_INFO (item_info_new(TYPE_ARGS(ArrayList *), (void *)arr_list_cmp, (void *)arr_list_copy, NULL, (void *)arr_list_free))

#pragma once
typedef struct ArrayList {
    size_t capacity;
    size_t size;
    void *data;
    ItemInfoTable itemInfo;
} ArrayList;

ArrayList *arr_list_new(size_t capacity, ItemInfoTable itemInfo);
void arr_list_free(ArrayList **arr);
void arr_list_resize(ArrayList *arr, size_t newSize);
void arr_list_set(ArrayList *arr, int64_t index, void *value);
void *arr_list_get(ArrayList *arr, int64_t index);
void arr_list_append(ArrayList *arr, void *value);
int arr_list_cmp(ArrayList *a, ArrayList *b);
void arr_list_copy(ArrayList *arr, ArrayList **dest);
void arr_list_shallow_copy(ArrayList *arr, ArrayList **dest);
int64_t arr_list_index_of(ArrayList *arr, void *value);
int64_t arr_list_Last_index_of(ArrayList *arr, void *value);
void *arr_list_min(ArrayList *arr);
void *arr_list_min(ArrayList *arr);

ArrayList *arr_list_new(size_t capacity, ItemInfoTable itemInfo) {
    ArrayList *arr = (ArrayList *)malloc(sizeof(ArrayList));

    arr->capacity = capacity;
    arr->size = 0;
    arr->data = calloc(capacity, itemInfo.size);
    arr->itemInfo = itemInfo;
    return arr;
}

void arr_list_free(ArrayList **arr) {
    if ((*arr)->itemInfo.free != NULL) {
        for (size_t i = 0; i < (*arr)->size; i++) {
            void **item = (void **)((char *)(*arr)->data + i * (*arr)->itemInfo.size);
            (*arr)->itemInfo.free(item);
        }
    }

    free((*arr)->data);
    free(*arr);
    *arr = NULL;
}

void arr_list_resize(ArrayList *arr, size_t newSize) {
    if (newSize >= arr->capacity) {
        arr->capacity = newSize * 2;
        void *newData = realloc(arr->data, arr->capacity * arr->itemInfo.size);

        if (newData) {
            arr->data = newData;
            memset((char *)arr->data + arr->size * arr->itemInfo.size, 0, newSize * arr->itemInfo.size);
        }
    }

    arr->size = newSize;
}

void arr_list_set_data(ArrayList *arr, size_t srcSize, void *src) {
    arr_list_resize(arr, srcSize / arr->itemInfo.size);
    memcpy(arr->data, src, srcSize);
}

void arr_list_set(ArrayList *arr, int64_t index, void *value) {
    memcpy((char *)arr->data + (index % arr->size) * arr->itemInfo.size, value, arr->itemInfo.size);
}

void *arr_list_get(ArrayList *arr, int64_t index) {
    return (void *)((char *)arr->data + (index % arr->size) * arr->itemInfo.size);
}

void arr_list_append(ArrayList *arr, void *value) {
    arr_list_resize(arr, arr->size + 1);
    memcpy((char *)arr->data + (arr->size - 1) * arr->itemInfo.size, value, arr->itemInfo.size);
}

int arr_list_cmp(ArrayList *a, ArrayList *b) {
    if (!a->itemInfo.comp || a->size == 0 || b->size == 0) {
        return -1;
    }

    int first = a->itemInfo.comp(arr_list_get(a, 0), arr_list_get(b, 0));

    for (size_t i = 1; i < a->size; i++) {
        if (a->itemInfo.comp(arr_list_get(a, i), arr_list_get(b, i)) != first) {
            return -1;
        }
    }

    return first;
}

void arr_list_copy(ArrayList *arr, ArrayList **dest) {
    *dest = arr_list_new(arr->capacity, arr->itemInfo);
    
    for (size_t i = 0; i < arr->size; i++) {
        void *itemCopy;

        arr->itemInfo.copy(*(void **)arr_list_get(arr, i), &itemCopy);
        arr_list_append(*dest, &itemCopy);
    }
}

void arr_list_shallow_copy(ArrayList *arr, ArrayList **dest) {
    *dest = arr_list_new(arr->capacity, arr->itemInfo);
    (*dest)->size = arr->size;
    memcpy((*dest)->data, arr->data, arr->size * arr->itemInfo.size);
}

int64_t arr_list_index_of(ArrayList *arr, void *value) {
    if (!arr->itemInfo.comp) {
        return -1;
    }

    for (size_t i = 0; i < arr->size; i++) {
        if (arr->itemInfo.comp(arr_list_get(arr, i), value) == 0) {
            return i;
        }
    }

    return -1;
}

int64_t arr_list_last_index_of(ArrayList *arr, void *value) {
    if (!arr->itemInfo.comp) {
        return -1;
    }

    for (size_t i = arr->size - 1; i >= arr->size; i--) {
        if (arr->itemInfo.comp(arr_list_get(arr, i), value) == 0) {
            return i;
        }
    }

    return -1;
}

void *arr_list_min(ArrayList *arr) {
    size_t minIndex = 0;

    for (size_t i = 0; i < arr->size; i++) {
        if (arr->itemInfo.comp(arr_list_get(arr, i), arr_list_get(arr, minIndex)) == -1) {
            minIndex = i;
        }
    }

    return arr_list_get(arr, minIndex);
}

void *arr_list_max(ArrayList *arr) {
    size_t minIndex = 0;

    for (size_t i = 0; i < arr->size; i++) {
        if (arr->itemInfo.comp(arr_list_get(arr, i), arr_list_get(arr, minIndex)) == 1) {
            minIndex = i;
        }
    }

    return arr_list_get(arr, minIndex);
}
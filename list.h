#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#pragma once
#ifdef __cplusplus
extern "C" {
#endif
typedef struct List {
    void** data;
    size_t capacity;
    size_t size;
} List;

static List* list_new(size_t initalCapacity) {
    List* list = (List*)malloc(sizeof(List));

    if (list == NULL) {
        return NULL;
    }

    list->data = (void**)calloc(initalCapacity, sizeof(void*));
    
    if (list->data == NULL && initalCapacity != 0) {
        free(list);
        return NULL;
    }

    list->capacity = initalCapacity;
    list->size = 0;
    return list;
}

static void* list_get(List* list, size_t index) {
    if (list != NULL && list->data != NULL) {
        return list->data[index % list->size];
    }

    return NULL;
}

static double list_get_num(List* list, size_t index) {
    if (list != NULL && list->data != NULL) {
        return ((double*)list->data)[index % list->size];
    }

    return 0.0;
}

static List* list_set(List* list, size_t index, void* item) {
    if (list != NULL && list->data != NULL) {
        list->data[index % list->size] = item;
        return list;
    }

    return NULL;
}

static List* list_safe_set(List* list, size_t index, void* item) {
    if (list != NULL && list->data != NULL) {
        if (index < list->size) {
            list->data[index] = item;
            return list;
        }
    }

    return NULL;
}

static List* list_set_num(List* list, size_t index, double num) {
    if (list != NULL && list->data != NULL) {
        ((double*)list->data)[index % list->size] = num;
        return list;
    }

    return NULL;
}

static List* list_safe_set_num(List* list, size_t index, double num) {
    if (list != NULL && list->data != NULL) {
        if (index < list->size) {
            ((double*)list->data)[index] = num;
            return list;
        }
    }

    return NULL;
}

static List* list_resize(List* list, size_t newSize) {
    if (list == NULL) {
        return NULL;
    }

    if (newSize <= list->capacity) {
        list->size = newSize;
    } else {
        size_t newCapacity = newSize * 2;
        void** newData = (void**)calloc(newCapacity, sizeof(void*));

        if (newData == NULL) {
            return NULL;
        }
        
        if (list->data != NULL) {
            memcpy(newData, list->data, (list->size < newSize ? list->size : newSize) * sizeof(void*));
            free(list->data);
        }

        list->data = newData;
        list->capacity = newCapacity;
        list->size = newSize;
    }

    return list;
}

static List* list_append(List* list, void* item) {
    if (list != NULL) {
        if (list_resize(list, list->size + 1)) {
            list->data[list->size - 1] = item;
            return list;
        }
    }

    return NULL;
}

static List* list_append_num(List* list, double num) {
    if (list != NULL) {
        if (list_resize(list, list->size + 1)) {
            ((double*)list->data)[list->size - 1] = num;
            return list;
        }
    }

    return NULL;
}

static List* list_clear(List* list) {
    if (list == NULL) {
        return NULL;
    }

    list->size = 0;
    return list;
}

static int list_free(List** list) {
    if (list == NULL) {
        return 0;
    }

    if ((*list)->data != NULL) {
        free((*list)->data);
    }

    free(*list);
    list = NULL;
    return 1;
}
#ifdef __cplusplus
}
#endif
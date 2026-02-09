#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "list.h"

#pragma once
#ifdef __cplusplus
extern "C" {
#endif
typedef struct Map {
    void** data;
    size_t capacity;
    size_t count;
    void* empty_address;
} Map;

static Map* map_new(size_t initalCapacity) {
    Map* map = (Map*)malloc(sizeof(Map));

    if (map == NULL) {
        return NULL;
    }

    map->data = (void**)calloc(initalCapacity, sizeof(void*));
    map->empty_address = (void*)malloc(1);
    
    if ((map->data == NULL && initalCapacity != 0) || map->empty_address == NULL) {
        free(map);
        return NULL;
    }

    map->capacity = initalCapacity;
    map->count = 0;
    return map;
}

static int map_resize(Map* map, size_t newCapacity) {
    if (map == NULL) {
        return 0;
    }

    void** newData = (void**)calloc(newCapacity, sizeof(void*));

    if (newData == NULL) {
        return 0;
    }
    
    if (map->data != NULL) {
        memcpy(newData, map->data, (map->capacity < newCapacity ? map->capacity : newCapacity) * sizeof(void*));
        free(map->data);
    }

    map->data = newData;
    map->capacity = newCapacity;

    return 1;
}

static size_t hash_value(const char* buff) {
    if (buff == NULL) {
        return 0;
    }

    size_t value = 0;

    while (*buff) {
        value = value * 151 + *buff++;
    }

    return value;
}

static int map_put(Map* map, const char* buff, void* item) {
    if (map == NULL) {
        return 0;
    }

    if (map->capacity == 0) {
        map_resize(map, 1024);
    } else if (map->count >= map->capacity / 2) {
        map_resize(map, map->capacity * 2);
    }

    size_t index = hash_value(buff) % map->capacity;

    if (map->data[index] != NULL) {
        return 0;
    }
    
    map->data[index] = item;
    map->count++;

    return 1;
}

static int map_put_num(Map* map, const char* buff, double num) {
    if (map == NULL) {
        return 0;
    }

    if (map->capacity == 0) {
        map_resize(map, 1024);
    } else if (map->count >= map->capacity / 2) {
        map_resize(map, map->capacity * 2);
    }

    size_t index = hash_value(buff) % map->capacity;

    if (map->data[index] != NULL) {
        return 0;
    }
    
    ((double*)map->data)[index] = num;
    map->count++;

    return 1;
}

static void* map_get(Map* map, const char* buff) {
    if (map == NULL) {
        return NULL;
    }

    if (map->count > 0 && map->data != NULL) {
        return map->data[hash_value(buff) % map->capacity];
    }

    return NULL;
}

static int map_set(Map* map, const char* buff, void* item) {
    if (map == NULL) {
        return 0;
    }

    if (map->data != NULL) {
        map->data[hash_value(buff) % map->capacity] = item;
    }

    return 1;
}

static double map_get_num(Map* map, const char* buff) {
    if (map == NULL) {
        return 0.0;
    }

    if (map->count > 0 && map->data != NULL) {
        return ((double*)map->data)[hash_value(buff) % map->capacity];
    }

    return 0.0;
}

static int map_set_num(Map* map, const char* buff, double num) {
    if (map == NULL) {
        return 0;
    }

    if (map->data != NULL) {
        ((double*)map->data)[hash_value(buff) % map->capacity] = num;
    }

    return 1;
}

static int map_clear(Map* map) {
    if (map == NULL) {
        return 0;
    }

    memset(map->data, 0, map->capacity);
    return 1;
}

static List* map_to_list(Map* map) {
    if (map == NULL) {
        return NULL;
    }

    double* data = (double*)map->data;
    List* list = list_new(map->capacity);

    for (int i = 0; i < map->capacity; i++) {
        if (data[i] != 0) {
            ((double*)list->data)[list->size] = data[i];
            list->size++;
        }
    }

    return list;
}

static int map_free(Map** map) {
    if (map == NULL) {
        return 0;
    }

    if ((*map)->data != NULL) {
        free((*map)->data);
    }

    if ((*map)->empty_address != NULL) {
        free((*map)->empty_address);
    }

    free(*map);
    map = NULL;
    return 1;
}
#ifdef __cplusplus
}
#endif
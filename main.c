#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "list.h"
#include "map.h"

int main(int argc, char* argv[]) {
    List* list = list_new(1024);

    list_append(list, (void*)"Hello, World! x (1)");
    list_append(list, (void*)"Hello, World! x (2)");
    list_append(list, (void*)"Hello, World! x (3)");
    list_append(list, (void*)"Hello, World! x (4)");

    for (size_t i = 0; i < list->size; i++) {
        printf("list[%zu] = %s\n", i, (char*)list_get(list, i));
    }

    printf("list->capacity = %zu\nlist->size = %zu\n", list->capacity, list->size);
    list_free(&list);
    
    // Has function
    printf("%zu\n", hash_value("Hello World"));
    printf("%zu\n", hash_value("hello world"));
    printf("%zu\n", hash_value("world hello"));
    printf("%zu\n", hash_value("dlrow olleh"));
    printf("%zu\n", hash_value("hello world123"));
    
    // Hashmap
    Map* map = map_new(1024);
    
    map_put(map, "hello", (void*)"Hello, World! x (1)");
    map_put(map, "hi", (void*)"Hello, World! x (2)");
    map_put(map, "hey", (void*)"Hello, World! x (3)");
    map_put(map, "myVar1", (void*)"10");
    map_put(map, "myVar2", (void*)"20");
    printf("map[\"hello\"] = %s\n", (char*)map_get(map, "hello"));
    printf("map[\"hi\"] = %s\n", (char*)map_get(map, "hi"));
    printf("map[\"hey\"] = %s\n", (char*)map_get(map, "hey"));
    printf("map[\"myVar1\"] = %s\n", (char*)map_get(map, "myVar1"));
    printf("map[\"myVar2\"] = %s\n", (char*)map_get(map, "myVar2"));

    List* map_list = map_to_list(map);

    for (size_t i = 0; i < map_list->size; i++) {
        printf("map_list[%zu] = %s\n", i, list_get(map_list, i));
    }

    list_free(&map_list);
    map_free(&map);
    
    // Int list
    List* num_list = list_new(0);
    
    list_append_num(num_list, 10.5);
    printf("%lf\n", list_get_num(num_list, 0));
    list_free(&num_list);

    return 0;
}
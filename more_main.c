#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "list.h"
#include "map.h"
#include "str.h"

int main(int argc, char* argv[]) {
    StringBuilder *str = sb_new("Hello, World!");
    
    sb_set_text(str, "Hello, World! New!");
    printf("text: %s\n", str->text);
    printf("text-size: %zu\n", str->size);
    sb_replace(str, " ", "Test");
    printf("string-replaced: %s\n", str->text);
    sb_reverse(str);
    printf("string-reversed: %s\n", str->text);

    StringBuilder *newStr = sb_new("test1-test2-test3");
    List *list = sb_split(newStr, "-");

    for (size_t i = 0; i < list->size; i++) {
        printf("list[%d]=%s\n", i, ((StringBuilder *)list_get(list, i))->text);
    }

    printf("Hello, World!\n");

    return 0;
}
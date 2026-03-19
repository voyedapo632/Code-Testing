
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "list.h"
#include "map.h"
#include "string_builder.h"

int main(int argc, char* argv[]) {
    StringBuilder *str = sb_new("Hello, World!");
    
    sb_set_text(str, "Hello, World! New!");
    printf("text: %s\n", str->text);
    printf("text-size: %zu\n", str->size);
    sb_replace(str, " ", "Test");
    sb_remove(str, "Test");
    printf("string-replaced: %s\n", str->text);

    StringBuilder *reversedStr = sb_remove_index(sb_replace(sb_reverse(sb_copy(str)), "!", "[END-TOKEN]"), 11);
    printf("string-reversed: %s\n", reversedStr->text);
    printf("original-string: %s\n", str->text);

    StringBuilder *newStr = sb_new("test1-test2-test3");
    List *list = sb_split(newStr, "-", -1);

    for (size_t i = 0; i < list->size; i++) {
        printf("list[%d]=%s\n", i, ((StringBuilder *)list_get(list, i))->text);
    }

    printf("Hello, World!\n");

    printf("Test: %s\n", sb_trim(sb_new("---Hello, World!--"), '-')->text);

    return 0;
}
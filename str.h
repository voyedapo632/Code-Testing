#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "list.h"

typedef struct StringBuilder {
    size_t capacity;
    size_t size;
    char *text;
} StringBuilder;

StringBuilder *sb_new(const char *text) {
    StringBuilder *str = (StringBuilder *)malloc(sizeof(StringBuilder));

    str->size = strlen(text);
    str->capacity = (str->size + 1) * 2;
    str->text = (char *)malloc(str->capacity);
    memcpy(str->text, text, str->size + 1);
    return str;
}

void sb_resize(StringBuilder *str, size_t newSize) {
    if (newSize >= str->capacity) {
        str->capacity = (newSize + 1) * 2;
        str->text = (char *)realloc(str->text, str->capacity);
    }

    str->size = newSize;
}

void sb_set_text(StringBuilder *str, const char* newText) {
    size_t newTextSize = strlen(newText);

    if (newTextSize >= str->capacity) {
        str->capacity = (newTextSize + 1) * 2;
        str->text = (char *)malloc(str->capacity);
    }

    str->size = newTextSize;
    memcpy(str->text, newText, str->size + 1);
}

StringBuilder *sb_copy(StringBuilder *str) {
    return sb_new(str->text);
}

char sb_get(StringBuilder *str, const size_t index) {
    return str->text[index % str->size];
}

void sb_set(StringBuilder *str, const size_t index, const char c) {
    str->text[index % str->size] = c;
}

void sb_append(StringBuilder *str, const char *newText) {
    size_t newTextSize = strlen(newText);
    size_t oldSize = str->size;

    sb_resize(str, str->size + newTextSize);
    memcpy(str->text + oldSize, newText, newTextSize);
}

int sb_equals(StringBuilder *str, const char* otherText) {
    size_t otherTextSize = strlen(otherText);

    if (otherTextSize != str->size) {
        return 0;
    }

    for (size_t i = 0; i < str->size; i++) {
        if (str->text[i] != otherText[i]) {
            return 0;
        }
    }

    return 1;
}

int sb_starts_with(StringBuilder *str, const char *otherText, size_t offset) {
    size_t otherTextSize = strlen(otherText);

    if (offset + otherTextSize > str->size) {
        return 0;
    }

    for (size_t i = 0; i < otherTextSize; i++) {
        if (str->text[offset + i] != otherText[i]) {
            return 0;
        }
    }

    return 1;
}

int64_t sb_index_of(StringBuilder *str, const char *substring) {
    size_t substringSize = strlen(substring);

    if (substringSize > str->size) {
        return -1;
    }

    size_t count = str->size - substringSize + 1;
    
    for (size_t i = 0; i < count; i++) {
        if (sb_starts_with(str, substring, i)) {
            return i;
        }
    }

    return -1;
}

int64_t sb_last_index_of(StringBuilder *str, const char *substring) {
    size_t substringSize = strlen(substring);

    if (substringSize > str->size) {
        return -1;
    }

    for (int64_t i = str->size - substringSize; i >= 0; i--) {
        if (sb_starts_with(str, substring, i)) {
            return i;
        }
    }

    return -1;
}

int sb_contains(StringBuilder *str, const char *substring) {
    size_t count = str->size - strlen(substring) + 1;
    
    for (size_t i = 0; i < count; i++) {
        if (sb_starts_with(str, substring, i)) {
            return 1;
        }
    }

    return 0;
}

List *sb_split(StringBuilder *str, const char *value) {
    size_t valueSize = strlen(value);
    List *result = list_new(1024);
    
    list_append(result, sb_new(""));

    for (size_t i = 0; i < str->size; i++) {
        if (sb_starts_with(str, value, i)) {
            list_append(result, sb_new(""));
            i += valueSize - 1;
        } else {
            char temp[2] = { str->text[i], '\0' };
            sb_append(((StringBuilder **)result->data)[result->size - 1], temp);
        }
    }

    return result;
}

void sb_replace(StringBuilder *str, const char *a, const char *b) {
    StringBuilder *tempStr = sb_new("");

    for (size_t i = 0; i < str->size; i++) {
        if (!sb_starts_with(str, a, i)) {
            char temp[2] = { str->text[i], '\0' };
            sb_append(tempStr, temp);
        } else {
            sb_append(tempStr, b);
        }
    }

    free(str->text);
    str->text = tempStr->text;
    str->capacity = tempStr->capacity;
    str->size = tempStr->size;
    free(tempStr);
}

void sb_remove(StringBuilder *str, const char* text) {
    sb_replace(str, text, "");
}

void sb_reverse(StringBuilder *str) {
    for (size_t i = 0; i < str->size / 2; i++) {
        const char temp = str->text[i];
        str->text[i] = str->text[str->size - i - 1];
        str->text[str->size - i - 1] = temp;
    }
}

void sb_clear(StringBuilder *str) {
    str->text[0] = '\0';
    str->size = 0;
}

void sb_free(StringBuilder **str) {
    free((*str)->text);
    free(*str);
    *str = NULL;
}

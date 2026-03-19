#include <stdio.h>

#include "array_list.h"
#include "dictionary.h"

int main(int argc, char *argv[]) {
    ArrayList *arr = arr_list_new(256, ARRAY_LIST_INFO);

    ArrayList *value = arr_list_new(256, INT_INFO);
    int num = 10;
    arr_list_append(value, &num);
    num = 20;
    arr_list_append(value, &num);
    num = 30;
    arr_list_append(value, &num);

    printf("%d\n", arr_list_index_of(value, &num));

    arr_list_append(arr, &value);

    for (size_t i = 0; i < arr->size; i++) {
        ArrayList *newArr = *(ArrayList **)arr_list_get(arr, i);

        for (size_t j = 0; j < newArr->size; j++) {
            int newNum = *(int *)arr_list_get(newArr, j);
            printf("[%d][%d] = %d\n", i, j, newNum);
        }
    }
    
    ArrayList *arrCopy;
    
    arr_list_copy(arr, &arrCopy);
    arr_list_free(&arr);

    for (size_t i = 0; i < arrCopy->size; i++) {
        ArrayList *newArr = *(ArrayList **)arr_list_get(arrCopy, i);
        
        for (size_t j = 0; j < newArr->size; j++) {
            int newNum = *(int *)arr_list_get(newArr, j);
            printf("[%d][%d] = %d\n", i, j, newNum);
        }
    }

    arr_list_free(&arrCopy);
    
    typedef struct KeyValuePairi32 {
        const char *key;
        int value;
    } KeyValuePairi32;

    KeyValuePairi32 pairs[] = {
        { "Sam", 10 },
        { "Tommy", 20 },
        { "John", 30 }
    };

    // Numeric array
    ArrayList *num_arr = arr_list_new(256, DOUBLE_INFO);

    double num_arr_data[] = { 10, -10, 3, 4, 20, 30 };
    arr_list_set_data(num_arr, sizeof(num_arr_data), num_arr_data);

    for (size_t i = 0; i < num_arr->size; i++) {
        printf("n%d = %f\n", i, *(double *)arr_list_get(num_arr, i));
    }

    printf("min = %f\n", *(double *)arr_list_min(num_arr));
    arr_list_free(&num_arr);

    // Dictionary
    Dictionary *dict = dict_new(64, DOUBLE_INFO);
    typedef struct { const char *key; double value; } Key;

    Key dictData[] = {
        { "n1", 10.0 },
        { "n2", 20.0 },
        { "n3", 30.0 }
    };

    dict_set_data(dict, sizeof(dictData), dictData);

    printf("dict[n1] = %f\n", *(double *)dict_get(dict, "n1"));
    printf("dict[n2] = %f\n", *(double *)dict_get(dict, "n2"));
    printf("dict[n3] = %f\n", *(double *)dict_get(dict, "n3"));
    return 0;
}
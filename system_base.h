
#include <stdio.h>
#include <stdint.h>

#define STRUCT_HEADER(t) typedef struct t t;
#define SYSTEM_ARGS(t) #t, sizeof(t), \
    (void *)t##_onStart, \
    (void *)t##_onBeginUpdate, \
    (void *)t##_onUpdate, \
    (void *)t##_onEndUpdate, \
    (void *)t##_onStop

STRUCT_HEADER(SystemBase)
struct SystemBase {
    int screen;
};

STRUCT_HEADER(SystemManager)
struct SystemManager {
    int screen;
};

void register_system(SystemManager *manager, const char *typeName, size_t size, 
    void *onStart, void *onBeginUpdate, 
    void *onUpdate, void *onEndUpdate, void *onStop) {
}
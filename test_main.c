
// Online C compiler to run C program online
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "RenderSystem.h"

typedef struct Vec3 {
    float x, y, z;
} Vec3;

Vec3 vec3_new(float x, float y, float z) {
    Vec3 v = { x, y, z };
    return v;
}

typedef uint32_t Entity;

typedef struct IComponent {
    char isActive; // Reserved byte for validation check
    const char *typeName;
} IComponent;

typedef struct Transform {
    IComponent base;
    Vec3 pos;
    Vec3 scale;
    Vec3 rot;
} Transform;

Transform transform_new(Vec3 pos, Vec3 scale, Vec3 rot) {
    Transform t = {
        { 1, "Transform" },
        pos,
        scale,
        rot
    };

    return t;
}

typedef struct EntityMisc {
    IComponent base;
    const char *name;
} EntityMisc;

typedef struct ComponentArray {
    const char *typeName;
    size_t typeSize;
    size_t size;
    char *data;
} ComponentArray;

ComponentArray *comp_arr_new(const char *typeName, size_t typeSize) {
    ComponentArray *arr = (ComponentArray *)malloc(sizeof(ComponentArray));

    arr->typeName = typeName;
    arr->typeSize = typeSize;
    arr->size = 0;
    arr->data = (char *)calloc(0, typeSize);
    return arr;
}

void comp_arr_resize(ComponentArray *arr, size_t newSize) {
    arr->data = (char *)realloc(arr->data, newSize * arr->typeSize);

    if (newSize > arr->size) {
        memset(arr->data + arr->size * arr->typeSize, 0, (newSize - arr->size) * arr->typeSize);
    }

    arr->size = newSize;
}

void comp_arr_set(ComponentArray *arr, size_t index, IComponent *c) {
    if (index >= arr->size) {
        comp_arr_resize(arr, index + 1);
    }

    memcpy(arr->data + index * arr->typeSize, c, arr->typeSize);
}

IComponent *comp_arr_get(ComponentArray *arr, size_t index) {
    if (index >= arr->size) {
        return NULL;
    }

    if (((IComponent *)(arr->data + index * arr->typeSize))->isActive) {
        return (IComponent *)(arr->data + index * arr->typeSize);
    }

    return NULL;
}

void comp_arr_zero(ComponentArray *arr, size_t index) {
    memset(arr->data + index * arr->typeSize, 0, arr->typeSize);
}

void comp_arr_free(ComponentArray **arr) {
    free((*arr)->data);
    free(*arr);
    *arr = NULL;
}

typedef struct EntityManager {
    Entity entitys;
    size_t componentsSize;
    ComponentArray **components;
} EntityManager;

EntityManager *ett_manager_new() {
    EntityManager *manager = (EntityManager *)malloc(sizeof(EntityManager));
    
    manager->entitys = 0;
    manager->componentsSize = 0;
    manager->components = (ComponentArray **)malloc(0);
    return manager;
}

void register_type(EntityManager *manager, const char *typeName, size_t typeSize) {
    manager->components = (ComponentArray **)realloc(manager->components, (manager->componentsSize + 1) * sizeof(ComponentArray *));
    manager->components[manager->componentsSize] = comp_arr_new(typeName, typeSize);
    manager->componentsSize++;
}

void add_component(EntityManager *manager, Entity e, IComponent *c) {
    for (size_t i = 0; i < manager->componentsSize; i++) {
        ComponentArray *arr = manager->components[i];

        if (arr->typeName == c->typeName) {
            comp_arr_set(arr, e, c);
            break;
        }
    }
}

// Returns a reference to the component
IComponent *get_component(EntityManager *manager, Entity e, const char *typeName) {
    for (size_t i = 0; i < manager->componentsSize; i++) {
        ComponentArray *arr = manager->components[i];

        if (arr->typeName == typeName) {
            return comp_arr_get(arr, e);
        }
    }

    return NULL;
}

Entity create_entity(EntityManager *manager) {
    return ++manager->entitys;
}

#define TYPE_ARGS(t) #t, sizeof(t)

typedef struct IComponentSystem {
    void(*onStart)(struct IComponentSystem *_this);
    void(*onUpdate)(struct IComponentSystem *_this, double deltaTime);
    void(*onEnd)(struct IComponentSystem *_this);
    void(*validate)(struct IComponentSystem *_this); // Searches for valid entities

    const char *typeName;
    EntityManager *manager;
    size_t validEntitiesSize;
    Entity *validEntities;
} IComponentSystem;

typedef struct IRenderComponentSystem {
    IComponentSystem base;
    uint32_t state;
} IRenderComponentSystem;

void onStart(IComponentSystem *_this) {
    IRenderComponentSystem *__this = (IRenderComponentSystem *)_this;
}

void onUpdate(IComponentSystem *_this, double deltaTime) {
    IRenderComponentSystem *__this = (IRenderComponentSystem *)_this;
}

void onEnd(IComponentSystem *_this) {
    IRenderComponentSystem *__this = (IRenderComponentSystem *)_this;
}

void validate(IComponentSystem *_this) {
    IRenderComponentSystem *__this = (IRenderComponentSystem *)_this;
}

IComponentSystem *render_comp_sys_new(EntityManager *manager) {
    IRenderComponentSystem *sys = (IRenderComponentSystem *)malloc(sizeof(IRenderComponentSystem));

    sys->base.typeName = "IRenderComponentSystem";
    sys->base.manager = manager;
    sys->base.validEntities = NULL;
    sys->base.validEntitiesSize = 0;
    return (IComponentSystem *)sys;
}

int main() {
    EntityManager *manager = ett_manager_new();

    // Register custom component types
    register_type(manager, TYPE_ARGS(Transform));
    
    // Create player
    Entity player = create_entity(manager);
    
    Transform playerTransform = transform_new(
        vec3_new(0.0f, 0.0f, 0.0f), // Position
        vec3_new(3.0f, 1.0f, 1.0f), // Scale
        vec3_new(0.0f, 0.0f, 0.0f)  // Rotation
    );
    add_component(manager, player, (IComponent *)&playerTransform);
    
    Transform *pt_ptr;

    if (pt_ptr = (Transform *)get_component(manager, player, "Transform"))  {
        printf("Player: %f, %f, %f\n", pt_ptr->scale.x, pt_ptr->scale.y, pt_ptr->scale.z);
    }

    // Create zombie
    Entity zombie = create_entity(manager);

    Transform zombieTransform = transform_new(
        vec3_new(0.0f, 0.0f, 0.0f), // Position
        vec3_new(5.0f, 1.0f, 1.0f), // Scale
        vec3_new(0.0f, 0.0f, 0.0f)  // Rotation
    );
    add_component(manager, zombie, (IComponent *)&zombieTransform);

    Transform *zt_ptr;

    if (zt_ptr = (Transform *)get_component(manager, zombie, "Transform"))  {
        printf("Zombie: %f, %f, %f\n", zt_ptr->scale.x, zt_ptr->scale.y, zt_ptr->scale.z);
    }

    SystemManager *sysManager;
    register_system(sysManager, SYSTEM_ARGS(RenderSystem));
    return 0;
}
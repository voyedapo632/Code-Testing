#include "system_base.h"

STRUCT_HEADER(RenderSystem)
struct RenderSystem {
    SystemBase base;
    int screenWidth;
    int screenHeight;
};

void RenderSystem_onStart(RenderSystem *self);
void RenderSystem_onBeginUpdate(RenderSystem *self);
void RenderSystem_onUpdate(RenderSystem *self);
void RenderSystem_onEndUpdate(RenderSystem *self);
void RenderSystem_onStop(RenderSystem *self);
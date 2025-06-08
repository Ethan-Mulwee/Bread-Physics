#ifndef BSCENE
#define BSCENE

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "bMath.hpp"

#include "framebuffer.hpp"

class Scene {
    public:
        
        void init();
    
        void render();
    
        FrameBuffer frameBuffer;
        bMath::float2 size;
};

#endif
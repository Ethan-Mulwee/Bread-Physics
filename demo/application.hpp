#ifndef BWINDOW
#define BWINDOW

#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "bMath.hpp"
#include "ext/iostream.hpp"

#include "uicontext.hpp"
#include "ui.hpp"
#include "scene.hpp"

class Application {
    public:
        Application(const char* name, int width, int height);

        ~Application();

        void loop(); 

    private: 

    GLFWwindow* window;
    UI ui;
    Scene scene;
    UIContext context;

    int mHeight;
    int mWidth;
    bMath::float2 mSize;
};

#endif
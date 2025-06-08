#ifndef BAPP
#define BAPP

#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "bMath.hpp"
#include "ext/iostream.hpp"

#include "b_window.hpp"

class Application {
    public:
        Application(int width, int height, const char* name);

        ~Application();

        void loop(); 

    private: 

    GLWindow window;

};

#endif
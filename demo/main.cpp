#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "bMath.hpp"
#include "ext/iostream.hpp"


GLFWwindow* createWindow(int width, int height, const char* name) {
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    if (!glfwInit()) {
        fprintf(stderr, "Failed to intialize GLFW\n");
        exit( EXIT_FAILURE );
    }

    GLFWwindow* glfwWindow = glfwCreateWindow(width, height, name, NULL, NULL);

    if (!glfwWindow) {
        fprintf(stderr , "Failed to open GLFW window \n");
        glfwTerminate();
        exit( EXIT_FAILURE);
    }

    glfwMakeContextCurrent(glfwWindow);
    glfwSwapInterval(1); // Enable vsync

    // glfwSetScrollCallback(glfwWindow, onScrollCallback);

    glewInit();

    std::cout << glGetString(GL_VERSION) << std::endl;

    return glfwWindow;
}

void updateWindow(GLFWwindow* glfwWindow) {
    glfwSwapBuffers(glfwWindow);
    glfwPollEvents();
}

void destroyWindow(GLFWwindow* glfwWindow) {
    // ImGui_ImplOpenGL3_Shutdown();
    // ImGui_ImplGlfw_Shutdown();
    // ImGui::DestroyContext();

    glfwDestroyWindow(glfwWindow);
    glfwTerminate(); 
}

int main() {
    GLFWwindow* glfwWindow = createWindow(500, 500, "window"); 

    while(!glfwWindowShouldClose(glfwWindow)) { 
        updateWindow(glfwWindow);
    }

    destroyWindow(glfwWindow);
}
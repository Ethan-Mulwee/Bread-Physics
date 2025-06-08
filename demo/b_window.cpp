#include "b_window.hpp"

#include <iostream>

void GLWindow::init(int width, int height, const char *title)
{
    m_title = title;
    m_width = width;
    m_height = height;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    if (!glfwInit()) {
        fprintf(stderr, "Failed to intialize GLFW\n");
        exit( EXIT_FAILURE );
    }

    glfwWindow = glfwCreateWindow(m_width,m_height, "Window", NULL, NULL);

    if (!glfwWindow) {
        fprintf(stderr , "Failed to open GLFW window \n");
        glfwTerminate();
        exit( EXIT_FAILURE);
    }

    glfwMakeContextCurrent(glfwWindow);
    glfwSwapInterval(1); // Enable vsync

    glewInit();

    std::cout << glGetString(GL_VERSION) << std::endl;
    
    ImGuiHelper::init(glfwWindow);
    m_scene.init();
}

void GLWindow::render() {
    glViewport(0, 0, m_width, m_height);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    ImGuiHelper::intialize_render();
    
    m_scene.render();

    m_properties.render();

    ImGuiHelper::render();

    glfwPollEvents();
    glfwSwapBuffers(glfwWindow);
}

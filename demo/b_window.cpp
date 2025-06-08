#include "b_window.hpp"

#include <iostream>

void GLWindow::init(int width, int height, const char *title)
{
    m_Title = title;
    m_Width = width;
    m_Height = height;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    if (!glfwInit()) {
        fprintf(stderr, "Failed to intialize GLFW\n");
        exit( EXIT_FAILURE );
    }

    m_GLFWWindow = glfwCreateWindow(m_Width,m_Height, "Window", NULL, NULL);

    if (!m_GLFWWindow) {
        fprintf(stderr , "Failed to open GLFW window \n");
        glfwTerminate();
        exit( EXIT_FAILURE);
    }

    glfwMakeContextCurrent(m_GLFWWindow);
    glfwSwapInterval(1); // Enable vsync

    glewInit();

    std::cout << glGetString(GL_VERSION) << std::endl;
    
    ImGuiHelper::init(m_GLFWWindow);
    m_SceneRenderer.init(800,600);
}

void GLWindow::render() {
    glViewport(0, 0, m_Width, m_Height);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    ImGuiHelper::intialize_render();
    
    m_SceneRenderer.render();

    m_PropertiesPanel.render();

    ImGuiHelper::render();

    glfwPollEvents();
    glfwSwapBuffers(m_GLFWWindow);
}

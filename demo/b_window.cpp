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

    glfwSetScrollCallback(m_GLFWWindow, onScrollCallback);

    glewInit();

    std::cout << glGetString(GL_VERSION) << std::endl;
    
    ImGuiHelperFunctions::init(m_GLFWWindow);
    s_SceneView.init(1920,1080);
}

void GLWindow::render() {
    glViewport(0, 0, m_Width, m_Height);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    ImGuiHelperFunctions::intializeRender();
    
    s_SceneView.render();

    m_PropertiesPanel.render(s_SceneView);

    ImGuiHelperFunctions::render();

    glfwPollEvents();
    glfwSwapBuffers(m_GLFWWindow);
    
    input();
}

SceneView GLWindow::s_SceneView;

void GLWindow::input()
{
    double x, y;
    glfwGetCursorPos(m_GLFWWindow, &x, &y);
    s_SceneView.onMove(x, y);
}


// this is static
void GLWindow::onScrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    s_SceneView.onScroll(yoffset);
}


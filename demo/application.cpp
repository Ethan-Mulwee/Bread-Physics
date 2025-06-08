#include "application.hpp"

Application::Application(int width, int height, const char *name)
{
    window.init(width, height, name);
}

Application::~Application() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window.m_GLFWWindow);
    glfwTerminate(); 
}

void Application::loop() {
    while (!glfwWindowShouldClose(window.m_GLFWWindow)) {
        window.render();
    }
}

#include "application.hpp"

Application::Application(const char *name, int width, int height)
{
    window.init(width, height, name);
}

Application::~Application() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window.glfwWindow);
    glfwTerminate(); 
}

void Application::loop() {
    while (!glfwWindowShouldClose(window.glfwWindow)) {
        window.render();
    }
}

#include "window.hpp"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

GLWindow *createWindow(int width, int height, const char *name) {

  GLWindow *window = new GLWindow();

  window->width = width;
  window->height = height;
  window->name = name;

  const char *glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  if (!glfwInit()) {
    fprintf(stderr, "Failed to intialize GLFW\n");
    exit(EXIT_FAILURE);
  }

  window->glfwWindow = glfwCreateWindow(width, height, name, NULL, NULL);

  if (!window->glfwWindow) {
    fprintf(stderr, "Failed to open GLFW window \n");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window->glfwWindow);
  glfwSwapInterval(1); // Enable vsync

  // glfwSetScrollCallback(glfwWindow, onScrollCallback);

  glewInit();

  std::cout << glGetString(GL_VERSION) << std::endl;
  std::cout << glGetString(GL_RENDERER) << std::endl;

  glfwSetWindowUserPointer(window->glfwWindow, window);
  glfwSetScrollCallback(window->glfwWindow, [](GLFWwindow *window,
                                               double xoffset, double yoffset) {
    GLWindow *windowA = (GLWindow *)glfwGetWindowUserPointer(window);
    windowA->scrollInput = yoffset;
  });

  return window;
}

void updateWindow(GLWindow *window) {
  window->scrollInput = 0.0f;

  glfwSwapBuffers(window->glfwWindow);
  glfwPollEvents();

  double x, y;
  glfwGetCursorPos(window->glfwWindow, &x, &y);
  smath::vector2 currentPos = smath::vector2{(float)x, (float)y};
  window->deltaMousePos = currentPos - window->mousePos;
  window->mousePos = currentPos;

  float currentTime = glfwGetTime();
  window->deltaTime = currentTime - window->time;
  window->time = currentTime;
}
 
void destroyWindow(GLWindow *window) {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window->glfwWindow);
  delete window;
  glfwTerminate();
}

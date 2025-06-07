#include <iostream>
#include <GLFW/glfw3.h>

int main() {
  std::cout << "Hello World! \n";

  GLFWwindow* window;
  if (!glfwInit()) {
    fprintf(stderr, "Failed to intialize GLFW\n");
    exit( EXIT_FAILURE );
  }

  window = glfwCreateWindow(500,500, "Window", NULL, NULL);

  if (!window) {
    fprintf(stderr , "Failed to open GLFW window \n");
    glfwTerminate();
    exit( EXIT_FAILURE);
  }

  while (!glfwWindowShouldClose(window)) {
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
}
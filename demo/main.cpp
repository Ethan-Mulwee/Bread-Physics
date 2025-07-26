#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "bMath.hpp"
#include "ext/iostream.hpp"

/* -------------------------------------------------------------------------- */
/*                                 GLFW Window                                */
/* -------------------------------------------------------------------------- */

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

/* -------------------------------------------------------------------------- */
/*                                    ImGui                                   */
/* -------------------------------------------------------------------------- */

void imGuiInit(GLFWwindow *window) {
    const char* glsl_version = "#version 130";

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

    // colors from jayanam
    auto& colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f };

    colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
    colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
    colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

    colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
    colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
    colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

    colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
    colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
    colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

    colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
    colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.38f, 0.38f, 1.0f };
    colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.28f, 0.28f, 1.0f };
    colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };

    colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
    colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void imGuiIntializeRender() {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    // Create the docking environment
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
    ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
    ImGuiWindowFlags_NoBackground;
    
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("InvisibleWindow", nullptr, windowFlags);
    ImGui::PopStyleVar(3);
    
    ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");
    
    ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::End();
}

void imGuiRender() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

/* -------------------------------------------------------------------------- */
/*                                   OpenGL                                   */
/* -------------------------------------------------------------------------- */

void openGLInit() {
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
}

/* -------------------------------------------------------------------------- */
/*                                 Framebuffer                                */
/* -------------------------------------------------------------------------- */

struct FrameBuffer {
    uint32_t fBO = 0;
    uint32_t texId = 0;
    uint32_t depthId = 0;
    uint32_t width = 0;
    uint32_t height = 0;
};

void unbindFramebuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void bindFramebuffer(const FrameBuffer &buffer) {
    glBindFramebuffer(GL_FRAMEBUFFER, buffer.fBO);
    glViewport(0,0, buffer.width, buffer.height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

FrameBuffer createFrameBuffer(int32_t width, int32_t height) {
    // ////////// HELLO TRIANGLE TESTING CODE
    // float positions[6] = {
    //     -0.5f, -0.5f, // 0
    //     0.0f, 0.5f, // 1
    //     0.5f, -0.5f, // 2
    // };
    // unsigned int indices[] {
    //     0, 1, 2,
    // };

    // unsigned int buffer;
    // glGenBuffers(1, &buffer);
    // glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // glBufferData(GL_ARRAY_BUFFER, 8*sizeof(float), positions, GL_STATIC_DRAW);

    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);
    
    // unsigned int ibo;
    // glGenBuffers(1, &ibo);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(float), indices, GL_STATIC_DRAW);
    // ////////// HELLO TRIANGLE TESTING CODE

    FrameBuffer buffer;

    buffer.width = width;
    buffer.height = height;

    glGenFramebuffers(1, &buffer.fBO);
    glBindFramebuffer(GL_FRAMEBUFFER, buffer.fBO);
    glCreateTextures(GL_TEXTURE_2D, 1, &buffer.texId);
    glBindTexture(GL_TEXTURE_2D, buffer.texId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, buffer.width, buffer.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer.texId, 0);

    glCreateTextures(GL_TEXTURE_2D, 1, &buffer.depthId);
    glBindTexture(GL_TEXTURE_2D, buffer.depthId);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, buffer.width, buffer.height);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, buffer.depthId, 0);

    GLenum buffers[4] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(buffer.texId, buffers);
    
    unbindFramebuffer();
}

/* -------------------------------------------------------------------------- */
/*                                VertexBuffer                                */
/* -------------------------------------------------------------------------- */

struct Vertex {
    bMath::float3 position;
    bMath::float3 normal;
};

struct VertexBuffer {
    std::vector<Vertex> vertices; 
    std::vector<uint32_t> indices;
    uint32_t vao, vbo, ebo;
};

void bindVertexBuffer(const VertexBuffer &buffer) {
    glBindVertexArray(buffer.vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ebo);
}

void unbindVertexBuffer() {
    glBindVertexArray(0);
}

VertexBuffer createVertexBuffer(std::vector<Vertex> vertices, std::vector<uint32_t> indices) {

    VertexBuffer buffer;

    buffer.vertices = vertices;
    buffer.indices = indices;

    glGenBuffers(1, &buffer.vbo);
    glGenBuffers(1, &buffer.ebo);
    glGenVertexArrays(1, &buffer.vao);
    
    glBindVertexArray(buffer.vao);

    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void drawVertexBuffer(const VertexBuffer &buffer) {
    bindVertexBuffer(buffer);
    glDrawElements(GL_TRIANGLES, buffer.indices.size(), GL_UNSIGNED_INT, nullptr);
    unbindVertexBuffer();
}



int main() {
    GLFWwindow* glfwWindow = createWindow(500, 500, "window"); 
    imGuiInit(glfwWindow);
    openGLInit();

    FrameBuffer frameBuffer = createFrameBuffer(1920, 1080);

    while(!glfwWindowShouldClose(glfwWindow)) { 
        updateWindow(glfwWindow);
    }

    destroyWindow(glfwWindow);
}
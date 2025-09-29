#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "smath.hpp"
#include "to_string.hpp"
#include "smath_iostream.hpp"

#include "bEngine/world.hpp"

/* -------------------------------------------------------------------------- */
/*                                 GLFW Window                                */
/* -------------------------------------------------------------------------- */

struct GLWindow {
    GLFWwindow* glfwWindow;
    int width, height;
    const char* name;
    
    smath::vector2 mousePos = {0.0f,0.0f};
    smath::vector2 deltaMousePos = {0.0f,0.0f};
    float scrollInput = 0.0f;
    float time = 0.0f;
    float deltaTime = 0.0f;
    double perviousRenderTime = 0.0;
};

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    std::cout << xoffset << ", " << yoffset << "\n";
}

GLWindow createWindow(int width, int height, const char* name) {

    GLWindow window;

    window.width = width;
    window.height = height;
    window.name = name;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    if (!glfwInit()) {
        fprintf(stderr, "Failed to intialize GLFW\n");
        exit( EXIT_FAILURE );
    }

    window.glfwWindow = glfwCreateWindow(width, height, name, NULL, NULL);

    if (!window.glfwWindow) {
        fprintf(stderr , "Failed to open GLFW window \n");
        glfwTerminate();
        exit( EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window.glfwWindow);
    glfwSwapInterval(1); // Enable vsync

    // glfwSetScrollCallback(glfwWindow, onScrollCallback);

    glewInit();

    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << glGetString(GL_RENDERER) << std::endl;

    glfwSetWindowUserPointer(window.glfwWindow, &window);
    glfwSetScrollCallback(window.glfwWindow, [](GLFWwindow* window,double xoffset, double yoffset) { 
        GLWindow* windowA = (GLWindow*)glfwGetWindowUserPointer(window);
        windowA->scrollInput = yoffset; 
    });

    return window;
}


void updateWindow(GLWindow &window) {
    window.scrollInput = 0.0f;

    glfwSwapBuffers(window.glfwWindow);
    glfwPollEvents();

    double x, y;
    glfwGetCursorPos(window.glfwWindow, &x, &y);
    smath::vector2 currentPos = smath::vector2{(float)x,(float)y};
    window.deltaMousePos = currentPos - window.mousePos;
    window.mousePos = currentPos;

    float currentTime = glfwGetTime();
    window.deltaTime = currentTime - window.time;
    window.time = currentTime;
}

void destroyWindow(const GLWindow &window) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window.glfwWindow);
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

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

}

void openGLIntializeRender(GLWindow window) {
    glViewport(0, 0, window.width, window.height);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

FrameBuffer createFrameBuffer(int32_t width, int32_t height) {

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

    return buffer;
}

/* -------------------------------------------------------------------------- */
/*                                  Geometry                                  */
/* -------------------------------------------------------------------------- */

struct Vertex {
    smath::vector3 position;
    smath::vector3 normal;
};

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};

Mesh createMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices) {
    Mesh mesh;

    mesh.vertices = vertices;
    mesh.indices = indices;

    return mesh;
}

Mesh generateMeshCube(const float size = 0.5f) {

    using namespace smath;

    std::vector<Vertex> vertices = {
        {vector3{0,0,0}, vector3{0,0,0}},
        {vector3{0,0,1}, vector3{0,0,1}},
        {vector3{0,1,0}, vector3{0,1,0}},
        {vector3{0,1,1}, vector3{0,1,1}},
        {vector3{1,0,0}, vector3{1,0,0}},
        {vector3{1,0,1}, vector3{1,0,1}},
        {vector3{1,1,0}, vector3{1,1,0}},
        {vector3{1,1,1}, vector3{1,1,1}}
    };

    for (int i = 0; i < 8; i++) {
        (vertices)[i].position -= vector3{0.5,0.5,0.5};
        (vertices)[i].position *= size;
    }

    std::vector<uint32_t> indices = {
        //Top
        2, 6, 7, 2, 3, 7, 
        //Bottom
        0, 4, 5, 0, 1, 5, 
        //Left
        0, 2, 6, 0, 4, 6, 
        //Right
        1, 3, 7, 1, 5, 7, 
        //Front
        0, 2, 3, 0, 1, 3, 
        //Back
        4, 6, 7, 4, 5, 7 
    };

    return createMesh(vertices, indices);
}

Mesh generateMeshPlane(const float size) {
    using namespace smath;

    std::vector<Vertex> vertices = {
        {vector3{0.0f,0.0f,0.0f}, vector3{0,1,0}},
        {vector3{0.0f,0.0f,size}, vector3{0,1,0}},
        {vector3{size,0.0f,0.0f}, vector3{0,1,0}},
        {vector3{size,0.0f,size}, vector3{0,1,0}},
    };

    for (int i = 0; i < 4; i++) {
        vertices[i].position -= vector3{size/2.0f, 0.0f, size/2.0f};
    }

    std::vector<uint32_t> indices = {
        0, 1, 2,
        2, 1, 3
    };

    return createMesh(vertices, indices);
}

/* -------------------------------------------------------------------------- */
/*                                VertexBuffer                                */
/* -------------------------------------------------------------------------- */

struct VertexBuffer {
    Mesh mesh;
    uint32_t vao, vbo, ebo;
};

void bindVertexBuffer(const VertexBuffer &buffer) {
    glBindVertexArray(buffer.vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ebo);
}

void unbindVertexBuffer() {
    glBindVertexArray(0);
}

VertexBuffer createVertexBuffer(Mesh mesh) {

    VertexBuffer buffer;

    buffer.mesh = mesh;

    glGenBuffers(1, &buffer.vbo);
    glGenBuffers(1, &buffer.ebo);
    glGenVertexArrays(1, &buffer.vao);
    
    glBindVertexArray(buffer.vao);

    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), mesh.vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(uint32_t), mesh.indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return buffer;
}

void drawVertexBuffer(const VertexBuffer &buffer) {
    bindVertexBuffer(buffer);
    glDrawElements(GL_TRIANGLES, buffer.mesh.indices.size(), GL_UNSIGNED_INT, nullptr);
    unbindVertexBuffer();
}

/* -------------------------------------------------------------------------- */
/*                                   Camera                                   */
/* -------------------------------------------------------------------------- */

struct Camera {
    smath::vector3 focus;

    float distance;
    float pitch, yaw;
    float aspect, fov, near, far;
};

Camera createCamera(smath::vector3 focus, float distance, float fov, float near, float far, float pitch = 0.0f, float yaw = 0.0f) {
    Camera camera;

    camera.focus = focus;
    camera.distance = distance;

    camera.fov = fov;
    camera.near = near;
    camera.far = far;

    camera.aspect = 1.0f;
    camera.pitch = pitch;
    camera.yaw = yaw;

    return camera;
}

smath::quaternion calculateCameraOrientation(const Camera &camera) {
    return smath::quaternion_from_euler_angles_ZYX(0.0f, camera.yaw, camera.pitch);
}

smath::vector3 calculateCameraPosition(const Camera &camera) {
    smath::quaternion orientation = calculateCameraOrientation(camera);

    smath::vector3 forwardVector = smath::quaternion_transform_vector(orientation, smath::vector3{0.0f, 0.0f, 1.0f});
    smath::vector3 position = camera.focus - (forwardVector * camera.distance);

    return position;
}

smath::matrix4x4 calculateCameraView(const Camera &camera) {

    using namespace smath;

    smath::quaternion orientation = calculateCameraOrientation(camera);

    smath::vector3 forwardVector = smath::quaternion_transform_vector(orientation, smath::vector3{0.0f, 0.0f, 1.0f});
    smath::vector3 position = camera.focus - (forwardVector * camera.distance);
    smath::matrix3x3 rotationMatrix = smath::matrix3x3_from_quaternion(orientation); /* smath::matrix3x3_from_identity(); */

    smath::matrix4x4 transformationMatrix = smath::matrix4x4_from_matrix3x3(transpose(rotationMatrix));

    position = matrix3x3_transform_vector3(transpose(rotationMatrix), position);

    transformationMatrix[0][3] = position.x;
    transformationMatrix[1][3] = position.y;
    transformationMatrix[2][3] = position.z;

    return transformationMatrix;
}

smath::matrix4x4 calculateCameraProjection(const Camera &camera) {
    return smath::matrix4x4_from_perspective(camera.fov, camera.aspect, camera.near, camera.far) /* * smath::matrix4x4_from_translation(smath::vector3{0,0,-5.2}) */;
}

/* -------------------------------------------------------------------------- */
/*                                   Shader                                   */
/* -------------------------------------------------------------------------- */

struct Shader {
    unsigned int programId;
};

unsigned int getCompiledShader(unsigned int shaderType, const char* shaderSource) {
    unsigned int shader_id = glCreateShader(shaderType);

    glShaderSource(shader_id, 1, &shaderSource, nullptr);
    glCompileShader(shader_id);

    GLint result;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);

        GLchar* strInfoLog = new GLchar[length + 1];
        glGetShaderInfoLog(shader_id, length, &length, strInfoLog);

        fprintf(stderr, "Compile error in shader: %s\n", strInfoLog);
        delete[] strInfoLog;
    }

    return shader_id;
}

Shader createShader(const std::string &vertShader, const std::string &fragShader) {

    Shader shader;

    std::ifstream is_vs(vertShader);
    if (!is_vs.is_open()) {
        std::cerr << "Error opening file: " << strerror(errno) << std::endl;
    }

    const std::string f_vs((std::istreambuf_iterator<char>(is_vs)), std::istreambuf_iterator<char>());

    std::ifstream is_fs(fragShader);
        if (!is_fs.is_open()) {
        std::cerr << "Error opening file: " << strerror(errno) << std::endl;
    }

    const std::string f_fs((std::istreambuf_iterator<char>(is_fs)), std::istreambuf_iterator<char>());

    shader.programId = glCreateProgram();

    unsigned int vs = getCompiledShader(GL_VERTEX_SHADER, f_vs.c_str());
    unsigned int fs = getCompiledShader(GL_FRAGMENT_SHADER, f_fs.c_str());

    glAttachShader(shader.programId, vs);
    glAttachShader(shader.programId, fs);

    GLint attached = 0;

    glLinkProgram(shader.programId);

    GLint linkResult;
    glGetProgramiv(shader.programId, GL_LINK_STATUS, &linkResult);
    if (linkResult == GL_FALSE) {
        std::cout << "shader linking failed \n";

        int length;
        glGetProgramiv(shader.programId, GL_INFO_LOG_LENGTH, &length);

        GLchar* strInfoLog = new GLchar[length + 1];
        glGetShaderInfoLog(shader.programId, length, &length, strInfoLog);

        fprintf(stderr, "Shader linking error: %s\n", strInfoLog);
        delete[] strInfoLog;
    }


    glValidateProgram(shader.programId);

    GLint validationResult;
    glGetProgramiv(shader.programId, GL_VALIDATE_STATUS, &validationResult);
    if (validationResult == GL_FALSE) {
        std::cout << "shader validation failed \n";
    }


    glDeleteShader(vs);
    glDeleteShader(fs);

    return shader;
}

void useShader(const Shader &shader) {
    glUseProgram(shader.programId);
}

void setShaderUniformMatrix4(const Shader &shader, const smath::matrix4x4 &mat4, const std::string &name) {
    GLint myLoc = glGetUniformLocation(shader.programId, name.c_str());
    glUniformMatrix4fv(myLoc, 1, GL_TRUE, &mat4.data[0][0]);
}

void setShaderUniformInt(const Shader &shader, const int v, const std::string &name) {
    GLint myLoc = glGetUniformLocation(shader.programId, name.c_str());
    glUniform1i(myLoc, v);
}

void setShaderUniformFloat3(const Shader &shader, const smath::vector3 &v, const std::string &name) {
    GLint myLoc = glGetUniformLocation(shader.programId, name.c_str());
    glProgramUniform3fv(shader.programId, myLoc, 1, (float*)&v);
}

void setShaderUniformFloat4(const Shader &shader, const smath::vector4 &v, const std::string &name) {
    GLint myLoc = glGetUniformLocation(shader.programId, name.c_str());
    glProgramUniform4fv(shader.programId, myLoc, 1, (float*)&v);
}

void setShaderUniformsFromCamera(const Shader &shader, const Camera &camera) {
    setShaderUniformMatrix4(shader, calculateCameraView(camera), "view");
    setShaderUniformMatrix4(shader, calculateCameraProjection(camera), "projection");
}

/* -------------------------------------------------------------------------- */
/*                                     UI                                     */
/* -------------------------------------------------------------------------- */

ImVec2 uiFrameBufferWindow(const FrameBuffer &frameBuffer) {
    ImGui::Begin("Scene");
    
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    
    // add rendered texture to ImGUI scene window
    uint64_t textureID = frameBuffer.texId;
    ImGui::Image((ImTextureID)(textureID), ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    
    ImGui::End();
    return viewportPanelSize;
}

void uiProperties(const GLWindow &window) {
    ImGuiIO& io = ImGui::GetIO();
    
    ImGui::Begin("Properties");
    ImGui::Text("Render time: %fms", window.perviousRenderTime*1000.0);
    if (ImGui::CollapsingHeader("Object", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("Hello World");

    }

    ImGui::End();
}

/* -------------------------------------------------------------------------- */
/*                               Render Commands                              */
/* -------------------------------------------------------------------------- */

enum RenderCommandType {
    Sphere,
    Vector
};

struct RenderCommand {
    RenderCommandType type;

    smath::vector3 positon;
    smath::vector3 direction;
    float radius;
    float length;

    smath::vector3 color;
};

struct RenderCommandBuffer {
    RenderCommand renderCommands[256];
    uint count = 0;

    void add(RenderCommand renderCommand) {
        renderCommands[count] = renderCommand;
        count++;
    }
};

void DrawSphere(RenderCommandBuffer &commandBuffer, const smath::vector3 &position, const float radius) {
    RenderCommand command = {
        .type = RenderCommandType::Sphere,
        .positon = position,
        .radius = radius
    };

    commandBuffer.add(command);
}

void DrawVector(RenderCommandBuffer &commandBuffer, const smath::vector3 &positon, const smath::vector3 &direction, const float length, const float radius, const smath::vector3 &color) {
    RenderCommand command = {
        .type = RenderCommandType::Vector,
        .positon = positon,
        .direction = direction,
        .radius = radius,
        .length = length,
        .color = color
    };

    commandBuffer.add(command);
}

/* -------------------------------------------------------------------------- */
/*                                OBJ Importer                                */
/* -------------------------------------------------------------------------- */

// Code adapted from https://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
Mesh importObj(const char* path) {
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<smath::vector3> temp_vertices;
    std::vector<smath::vector2> temp_uvs;
    std::vector<smath::vector3> temp_normals;

    Mesh mesh;

    FILE* file = fopen(path, "r");
    if (file == NULL) {
        std::cout << "Error opening file \n";
        return mesh;
    }

    while(true) {
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;
        
        if (strcmp(lineHeader, "v") == 0) {
            smath::vector3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        } else if (strcmp(lineHeader, "vt") == 0) {
            smath::vector2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            temp_uvs.push_back(uv);
        } else if (strcmp(lineHeader, "vn") == 0) {
            smath::vector3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
        } else if (strcmp(lineHeader, "f") == 0) {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(
                file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
                &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                &vertexIndex[2], &uvIndex[2], &normalIndex[2]
            );
            if (matches != 9) {
                std::cout << "Parsing error \n";
                return mesh;
            }

            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);

        }
    }
    for (int i = 0; i < vertexIndices.size(); i++) {
        unsigned int vertexIndex = vertexIndices[i];
        unsigned int normalIndex = normalIndices[i];
        smath::vector3 position = temp_vertices[vertexIndex-1];
        smath::vector3 normal = temp_normals[normalIndex-1];

        Vertex vertex;
        vertex.position = position;
        vertex.normal = normal;
        mesh.vertices.push_back(vertex);
        mesh.indices.push_back(i);
    }
    // for (int i = 0; i < vertexIndices.size(); i++) {
    //     vertexIndices[i] = vertexIndices[i]-1;
    // }
    // std::cout << temp_vertices.size() << "\n";
    // for (int i = 0; i < temp_vertices.size(); i++) {
    //     Vertex vertex;
    //     vertex.position = temp_vertices[i];
    //     vertex.normal = temp_vertices[i];
    //     mesh.vertices.push_back(vertex);
    //     unsigned int vertexIndex = vertexIndices[i]-1;
    //     mesh.indices = vertexIndices;
    // }
    return mesh;
}

/* -------------------------------------------------------------------------- */
/*                                   Objects                                  */
/* -------------------------------------------------------------------------- */

struct Object {
    VertexBuffer buffer;

    smath::vector4 color;
    smath::matrix4x4 transform;
};

Object createObject(const VertexBuffer &buffer) {
    Object object;
    object.buffer = buffer;

    object.color = smath::vector4{1.0f,1.0f,1.0f,1.0f};
    object.transform = smath::matrix4x4_from_identity();

    return object;
}

void drawObject(const Object &object, const Shader &shader) {
    useShader(shader);
    setShaderUniformMatrix4(shader, object.transform, "model");
    setShaderUniformFloat4(shader, object.color, "color");
    drawVertexBuffer(object.buffer);
}

void drawRenderCommand(const RenderCommand &command, Object &spherePrimitiveObject, const Shader &shader) {
    switch (command.type) {
        case RenderCommandType::Sphere:
            {
                smath::transform transform{
                    .translation = command.positon,
                    .rotation = smath::quaternion{0.0f, 0.0f, 0.0f, 1.0f},
                    .scale = smath::vector3{command.radius,command.radius,command.radius}
                };
                spherePrimitiveObject.transform = smath::matrix4x4_from_transform(transform);
                drawObject(spherePrimitiveObject, shader);
                break;           
            }
        case RenderCommandType::Vector:
            {
                break;
            }
    }
}

struct PhysicsObject {
    Object* object;
    bEngine::RigidBody* rigidBody;
};

/* -------------------------------------------------------------------------- */
/*                                 Application                                */
/* -------------------------------------------------------------------------- */

struct Scene {
    FrameBuffer* frameBuffer;
    VertexBuffer* vertexBuffer;
};


void render(
    const GLWindow &window, 
    const FrameBuffer &frameBuffer, 
    const std::vector<Object> &objects, 
    const Shader &objectShader, 
    const Shader &gridShader, 
    Object &gridObject, 
    Camera &camera, 
    RenderCommandBuffer &commandBuffer,
    Object &primtiveSphere
) {
    openGLIntializeRender(window);
    imGuiIntializeRender();

    
    
    bindFramebuffer(frameBuffer);

        glDisable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        useShader(objectShader);
        setShaderUniformsFromCamera(objectShader, camera);

        for (int i = 0; i < objects.size(); i++) {
            drawObject(objects[i], objectShader);
        }

        for (int i = commandBuffer.count; i > 0;  i--) {
            drawRenderCommand(commandBuffer.renderCommands[i-1], primtiveSphere, objectShader);
        }
        commandBuffer.count = 0;


        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glDisable(GL_CULL_FACE);

        useShader(gridShader);
        setShaderUniformsFromCamera(gridShader, camera);
        drawObject(gridObject, gridShader);

        smath::vector3 cameraPosition = calculateCameraPosition(camera);
        gridObject.transform[0][3] = -cameraPosition.x;
        gridObject.transform[2][3] = -cameraPosition.z;


    unbindFramebuffer();

    ImVec2 frameSize = uiFrameBufferWindow(frameBuffer);
    camera.aspect = frameSize.x / frameSize.y;

    uiProperties(window);

    imGuiRender();
}

int main() {
    GLWindow window = createWindow(1920, 1080, "window"); 
    RenderCommandBuffer renderCommandBuffer;
    imGuiInit(window.glfwWindow);
    openGLInit();
    
    bEngine::World physicsWorld;

    Shader objectShader = createShader("../demo/shaders/shader.vert", "../demo/shaders/shader.frag");
    Shader gridShader = createShader("../demo/shaders/shader.vert", "../demo/shaders/grid.frag");
    FrameBuffer frameBuffer = createFrameBuffer(1920, 1080);    
    std::vector<Object> objects;

    //// Create objects

    Mesh suzanneMesh = importObj("../demo/OBJs/Rounded-Cube.obj");
    VertexBuffer suzanneVertexBuffer = createVertexBuffer(suzanneMesh);

    Object suzanneObject = createObject(suzanneVertexBuffer);

    smath::transform suzanneTransform = {
        .translation = smath::vector3{-1.0f,0.5f,0.0f},
        .rotation = smath::quaternion_from_euler_angles_XYZ(-0.6f,0.0f,0.0f),
        .scale = smath::vector3{1.0f,1.0f,1.0f}
    };

    suzanneObject.transform = smath::matrix4x4_from_transform(suzanneTransform);
    suzanneObject.color = smath::vector4{1.000f,0.200f,0.322f,1.0f};

    objects.push_back(suzanneObject);

    Mesh teapotMesh = importObj("../demo/OBJs/Rounded-Cube.obj");
    VertexBuffer teapotVertexBuffer = createVertexBuffer(teapotMesh);

    Object teapotObject = createObject(teapotVertexBuffer);

    smath::transform teapotTransform = {
        .translation = smath::vector3{1.0f,0.0f,0.6f},
        .rotation = smath::quaternion_from_euler_angles_XYZ(0.0f,1.0f,0.0f),
        .scale = smath::vector3{0.4f,0.4f,0.4f}
    };
    
    teapotObject.transform = smath::matrix4x4_from_transform(teapotTransform);
    teapotObject.color = smath::vector4{0.157f,0.565f,1.0f,1.0f};

    objects.push_back(teapotObject);

    Mesh planeMesh = generateMeshPlane(250.0f);
    VertexBuffer planeVertexBuffer = createVertexBuffer(planeMesh);
    Object planeObject = createObject(planeVertexBuffer);

    Mesh spherePrimitiveMesh = importObj("../demo/OBJs/Primitive-Sphere.obj");
    VertexBuffer spherePrimitiveBuffer = createVertexBuffer(spherePrimitiveMesh);
    Object spherePrimitiveObject = createObject(spherePrimitiveBuffer);

    // objects.push_back(planeObject);

    //// Create objects

    // Cube1 ///////////////////////////////////////////////////////////////////////////////
    bEngine::RigidBody* body = new bEngine::RigidBody();
    body->inverseMass = 0.5f;
    body->inverseInertiaTensor = bMath::inverse(bMath::InertiaTensorCuboid(2,1,1,1));
    body->position = bMath::float3(0,1,0);
    // body->orientation = bMath::quaternion(0.951,0.189,0.198,-0.146);
    body->orientation = bMath::quaternion::identity();
    body->orientation.normalize();

    bEngine::Primitive collider;
    collider.type = bEngine::PrimitiveType::Cube;
    collider.dimensions = bMath::float3(0.5,0.5,0.5);

    collider.offset = bMath::matrix4::identity();
    collider.body = body;

    physicsWorld.bodies.push_back(body);
    physicsWorld.colliders.push_back(collider);
    ///////////////////////////////////////////////////////////////////////////////////////

    // Cube2 ///////////////////////////////////////////////////////////////////////////////
    bEngine::RigidBody* body2 = new bEngine::RigidBody();
    body2->inverseMass = 0.5f;
    body2->inverseInertiaTensor = bMath::inverse(bMath::InertiaTensorCuboid(2,1,1,1));
    body2->position = bMath::float3(0,3.2,0);
    body2->orientation = bMath::quaternion(1,0,0,0);
    body2->angularVelocity = bMath::float3(0,0,0);

    bEngine::Primitive collider2;
    collider2.type = bEngine::PrimitiveType::Cube;
    collider2.dimensions = bMath::float3(0.5,0.5,0.5);
    collider2.offset = bMath::matrix4::identity();
    collider2.body = body2;

    physicsWorld.bodies.push_back(body2);
    physicsWorld.colliders.push_back(collider2);
    ///////////////////////////////////////////////////////////////////////////////////////

    Camera camera = createCamera(smath::vector3{0.0f,0.0f,0.0f}, 5.0f, 45.0f, 0.1f, 100.0f, -M_PI/4.0f, M_PI/4.0f);

    while(!glfwWindowShouldClose(window.glfwWindow)) { 
        updateWindow(window);

        if (glfwGetMouseButton(window.glfwWindow, GLFW_MOUSE_BUTTON_MIDDLE) && !glfwGetKey(window.glfwWindow, GLFW_KEY_LEFT_SHIFT)) {
            camera.yaw -= window.deltaMousePos.x*0.0075f;
            camera.pitch -= window.deltaMousePos.y*0.0075f;
        }   
        
        if (glfwGetMouseButton(window.glfwWindow, GLFW_MOUSE_BUTTON_MIDDLE) && glfwGetKey(window.glfwWindow, GLFW_KEY_LEFT_SHIFT)) {
            smath::vector3 movement = smath::vector3{-window.deltaMousePos.x,window.deltaMousePos.y,0.0f};
            smath::quaternion cameraOrientation = calculateCameraOrientation(camera);

            movement = smath::quaternion_transform_vector(cameraOrientation, movement);
            movement *= 0.00075f;

            camera.focus -= movement*camera.distance;
        }   

        camera.distance -= window.scrollInput*camera.distance*0.075f;

        bMath::matrix4 btransform0 = physicsWorld.bodies[0]->getTransform();
        bMath::matrix4 btransform1 = physicsWorld.bodies[1]->getTransform();
        smath::matrix4x4 transform0 = *(smath::matrix4x4*)&(btransform0);
        smath::matrix4x4 transform1 = *(smath::matrix4x4*)&(btransform1);
        smath::matrix4x4 scaling = smath::matrix4x4_from_diagonal(0.5f);
        scaling[3][3] = 1.0f;
        transform0 = transform0 * scaling;
        transform1 = transform1 * scaling;

        objects[0].transform = transform0;
        objects[1].transform = transform1;

        DrawSphere(renderCommandBuffer, smath::vector3{1.0f,1.0f,0.0f}, 0.1f);
        DrawSphere(renderCommandBuffer, smath::vector3{1.0f,2.0f,0.0f}, 0.1f);

        for (int i = 0; i < physicsWorld.bodies.size(); i++) {
          physicsWorld.bodies[i]->addForce(bMath::float3(0,-9.8,0)*(1.0f/physicsWorld.bodies[i]->inverseMass));
        }

        physicsWorld.step(window.deltaTime*0.5f);

        double beforeTime = glfwGetTime();
        render(window, frameBuffer, objects, objectShader, gridShader, planeObject, camera, renderCommandBuffer, spherePrimitiveObject);
        double afterTime = glfwGetTime();
        
        window.perviousRenderTime = afterTime - beforeTime;
    }

    destroyWindow(window);
}
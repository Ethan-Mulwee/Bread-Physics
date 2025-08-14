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

/* -------------------------------------------------------------------------- */
/*                                 GLFW Window                                */
/* -------------------------------------------------------------------------- */

struct GLWindow {
    GLFWwindow* glfwWindow;
    int width, height;
    const char* name;
};

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

    return window;
}

void updateWindow(const GLWindow &window) {
    glfwSwapBuffers(window.glfwWindow);
    glfwPollEvents();
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
    std::vector<Vertex>* vertices;
    std::vector<uint32_t>* indices;
};

Mesh createMesh(std::vector<Vertex>* vertices, std::vector<uint32_t>* indices) {
    Mesh mesh;

    mesh.vertices = vertices;
    mesh.indices = indices;

    return mesh;
}

Mesh genMeshCube(const float size = 0.5f) {

    using namespace smath;

    std::vector<Vertex>* testVertices = new std::vector<Vertex>{
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
        (*testVertices)[i].position -= vector3{0.5,0.5,0.5};
        (*testVertices)[i].position *= size;
    }

    std::vector<uint32_t>* testIndices = new std::vector<uint32_t>{
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

    return createMesh(testVertices, testIndices);
}

/* -------------------------------------------------------------------------- */
/*                                VertexBuffer                                */
/* -------------------------------------------------------------------------- */

struct VertexBuffer {
    Mesh mesh;
    smath::matrix4x4 transform;
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
    buffer.transform = smath::matrix4x4_from_identity();

    glGenBuffers(1, &buffer.vbo);
    glGenBuffers(1, &buffer.ebo);
    glGenVertexArrays(1, &buffer.vao);
    
    glBindVertexArray(buffer.vao);

    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices->size() * sizeof(Vertex), mesh.vertices->data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices->size() * sizeof(uint32_t), mesh.indices->data(), GL_STATIC_DRAW);

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
    glDrawElements(GL_TRIANGLES, buffer.mesh.indices->size(), GL_UNSIGNED_INT, nullptr);
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

Camera createCamera(smath::vector3 focus, float distance, float fov, float near, float far) {
    Camera camera;

    camera.focus = focus;
    camera.distance = distance;

    camera.fov = fov;
    camera.near = near;
    camera.far = far;

    camera.aspect = 1.0f;
    camera.pitch = 0.0f;
    camera.yaw = 0.0f;

    return camera;
}

smath::quaternion calculateCameraOrientation(const Camera &camera) {
    return smath::quaternion_from_euler_angles_ZYX(0.0f, camera.yaw, camera.pitch);
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

void setShaderUniformFloat3(const Shader &shader, smath::vector3 &v, const std::string &name) {
    GLint myLoc = glGetUniformLocation(shader.programId, name.c_str());
    glProgramUniform3fv(shader.programId, myLoc, 1, (float*)&v);
}

void setShaderUniformFloat4(const Shader &shader, smath::vector4 &v, const std::string &name) {
    GLint myLoc = glGetUniformLocation(shader.programId, name.c_str());
    glProgramUniform4fv(shader.programId, myLoc, 1, (float*)&v);
}

void setShaderUniformsFromCamera(const Shader &shader, const Camera &camera) {
    setShaderUniformMatrix4(shader, calculateCameraView(camera), "view");
    setShaderUniformMatrix4(shader, calculateCameraProjection(camera), "projection");
}

// TODO: make a model struct instead of usign a vertex buffer for this
void setShaderUniformsFromModel(const Shader &shader, const VertexBuffer &buffer) {
    setShaderUniformMatrix4(shader, buffer.transform, "model");
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

void uiProperties() {
    ImGuiIO& io = ImGui::GetIO();
    
    ImGui::Begin("Properties");
    if (ImGui::CollapsingHeader("Object", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("Hello World");

    }

    ImGui::End();
}

/* -------------------------------------------------------------------------- */
/*                                 Application                                */
/* -------------------------------------------------------------------------- */

struct Scene {
    FrameBuffer* frameBuffer;
    VertexBuffer* vertexBuffer;
};


void render(const GLWindow &window, const FrameBuffer &frameBuffer, const VertexBuffer &vertexBuffer, const Shader &shader, Camera &camera) {
    openGLIntializeRender(window);
    imGuiIntializeRender();

    useShader(shader);

    setShaderUniformsFromCamera(shader, camera);

    setShaderUniformsFromModel(shader, vertexBuffer);

    bindFramebuffer(frameBuffer);

        drawVertexBuffer(vertexBuffer);

    unbindFramebuffer();

    ImVec2 frameSize = uiFrameBufferWindow(frameBuffer);
    camera.aspect = frameSize.x / frameSize.y;

    uiProperties();

    imGuiRender();
}

int main() {
    GLWindow window = createWindow(1920, 1080, "window"); 
    imGuiInit(window.glfwWindow);
    openGLInit();

    Shader shader = createShader("../demo/shaders/shader.vert", "../demo/shaders/shader.frag");
    FrameBuffer frameBuffer = createFrameBuffer(1920, 1080);
    Mesh cubeMesh = genMeshCube(0.2f);
    VertexBuffer vertexBuffer = createVertexBuffer(cubeMesh);

    Camera camera = createCamera(smath::vector3{0.0f,0.0f,0.0f}, 0.5f, 45.0f, 0.1f, 100.0f);

    while(!glfwWindowShouldClose(window.glfwWindow)) { 
        updateWindow(window);

        camera.yaw += 0.02f;
        camera.pitch += 0.01f;        

        render(window, frameBuffer, vertexBuffer, shader, camera);
    }

    destroyWindow(window);
}
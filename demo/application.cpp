#include "application.hpp"

Application::Application(const char *name, int width, int height)
{
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    if (!glfwInit()) {
        fprintf(stderr, "Failed to intialize GLFW\n");
        exit( EXIT_FAILURE );
    }

    window = glfwCreateWindow(1920,1080, "Window", NULL, NULL);

    if (!window) {
        fprintf(stderr , "Failed to open GLFW window \n");
        glfwTerminate();
        exit( EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    glewInit();

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[6] = {
        -0.5f, -0.5f, // 0
        0.0f, 0.5f, // 1
        0.5f, -0.5f, // 2
    };
    unsigned int indices[] {
        0, 1, 2,
    };

    // Create and bind vertex buffers

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 8*sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);
    
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(float), indices, GL_STATIC_DRAW);

    // Create and bind frame buffer
    mWidth = width;
    mHeight = height;


    glGenFramebuffers(1, &mFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
    glCreateTextures(GL_TEXTURE_2D, 1, &mTexId);
    glBindTexture(GL_TEXTURE_2D, mTexId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexId, 0);

    glCreateTextures(GL_TEXTURE_2D, 1, &mDepthId);
    glBindTexture(GL_TEXTURE_2D, mDepthId);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, mWidth, mHeight);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthId, 0);

    GLenum buffers[4] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(mTexId, buffers);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ///////

    ui.intialize(window);

    context = UIContext();
    // Our state
    context.show_demo_window = true;
    context.show_another_window = false;
    context.clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
}

Application::~Application() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate(); 
}

void Application::loop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        // pre render UI
        ui.intialize_render();
        //////////

        // scene view render
        
        // bind frame buffer
        glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
        glViewport(0,0, mWidth, mHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // render mesh
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        
        // unbind buffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        // imgui code
        ImGui::Begin("Scene");
        
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        mSize = { viewportPanelSize.x, viewportPanelSize.y };
        
        // add rendered texture to ImGUI scene window
        uint64_t textureID = mTexId;
        ImGui::Image((ImTextureID)(textureID), ImVec2{ mSize.x, mSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        
        ImGui::End();
        
        //////////
        ImGuiIO& io = ImGui::GetIO();
        
        ImGui::Begin("Properties");
        if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
        {
            
        if (ImGui::Button("Open..."))
        {
            // mFileDialog.Open();
        }
        ImGui::SameLine(0, 5.0f);
        ImGui::Text("mCurrentFile.c_str()");
        }

        if (ImGui::CollapsingHeader("Material") && true)
        {
        // ImGui::ColorPicker3("Color", (float*)&mesh->mColor, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);
        // ImGui::SliderFloat("Roughness", &mesh->mRoughness, 0.0f, 1.0f);
        // ImGui::SliderFloat("Metallic", &mesh->mMetallic, 0.0f, 1.0f);
        }

        if (ImGui::CollapsingHeader("Light"))
        {

        ImGui::Separator();
        ImGui::Text("Position");
        ImGui::Separator();
        // nimgui::draw_vec3_widget("Position", scene_view->get_light()->mPosition, 80.0f);
        }

        ImGui::End();
        

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(context.clear_color.x * context.clear_color.w, context.clear_color.y * context.clear_color.w, context.clear_color.z * context.clear_color.w, context.clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
        }

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }


        glfwSwapBuffers(window);
    }
}

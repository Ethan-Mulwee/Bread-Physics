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


    // Create and bind vertex buffers
    scene.init();

    ui.init(window);

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
        scene.render();
        
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

#include "properties_panel.hpp"

void PropertiesPanel::render(SceneView &sceneView) {
    ImGuiIO& io = ImGui::GetIO();
    
    ImGui::Begin("Properties");
    if (ImGui::CollapsingHeader("Object", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("Hello World");

        ImGui::Text("Camera Position: (%g, %g, %g)", sceneView.m_Camera.getPosition().x, sceneView.m_Camera.getPosition().y, sceneView.m_Camera.getPosition().z);
        
        // if (ImGui::Button("Do stuff...")) {

        // }
        // ImGui::SameLine(0, 5.0f);
        // ImGui::Text("(some text)");
    }

    // if (ImGui::CollapsingHeader("Material") && true) {
    //     ImGui::ColorPicker3("Color", (float*)&color, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB);
    // }

    // if (ImGui::CollapsingHeader("Light"))
    // {

    //     ImGui::Separator();
    //     ImGui::Text("Position");
    //     ImGui::Separator();
    // }

    ImGui::End();
}
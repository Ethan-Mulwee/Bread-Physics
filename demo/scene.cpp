#include "scene.hpp"

void Scene::init()
{
    frameBuffer.create_buffers(800,600);
}

void Scene::render()
{
    // bind frame buffer
    frameBuffer.bind();
    
    
    // render mesh
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    
    // unbind buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // imgui code
    ImGui::Begin("Scene");
    
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    size = { viewportPanelSize.x, viewportPanelSize.y };
    
    // add rendered texture to ImGUI scene window
    uint64_t textureID = frameBuffer.get_texture();
    ImGui::Image((ImTextureID)(textureID), ImVec2{ size.x, size.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    
    ImGui::End();
}
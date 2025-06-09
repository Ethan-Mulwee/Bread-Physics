#include "scene_view.hpp"
#include <vector>

void SceneView::init(int32_t width, int32_t height)
{
    frameBuffer.create_buffers(width,height);

    using namespace bMath;

    // quaternion rotation(34, float3(0.5f,0.3f,0.0f));

    std::vector<vector3> testVertices = {
        vector3(0,0,0),
        vector3(0,0,1),
        vector3(0,1,0),
        vector3(0,1,1),
        vector3(1,0,0),
        vector3(1,0,1),
        vector3(1,1,0),
        vector3(1,1,1)
    };

    for (int i = 0; i < 8; i++) {
        testVertices[i] -= vector3(0.5,0.5,0.5);
        testVertices[i] *= 0.5f;
        // test rotation
        // testVertices[i] = testVertices[i] * rotation;
    }

    std::vector<uint32_t> testIndices = {
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
    vertexBuffer.create_buffers(testVertices, testIndices);
}

void SceneView::render()
{
    frameBuffer.bind();
    
    vertexBuffer.draw();
    
    frameBuffer.unbind();
    
    // imgui code
    ImGui::Begin("Scene");
    
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    size = { viewportPanelSize.x, viewportPanelSize.y };
    
    // add rendered texture to ImGUI scene window
    uint64_t textureID = frameBuffer.get_texture();
    ImGui::Image((ImTextureID)(textureID), ImVec2{ size.x, size.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    
    ImGui::End();
}
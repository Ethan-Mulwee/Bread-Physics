#include "scene_view.hpp"
#include <vector>

void SceneView::init(int32_t width, int32_t height)
{
    frameBuffer.createBuffers(width,height);

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
        // testVertices[i] = testVertices[i] * rotation;
        testVertices[i] *= 0.2f;
        // testVertices[i] -= vector3(0,0,0.5);
        // test rotation
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

    vertexBuffer.createBuffers(testVertices, testIndices);

    // fix these paths, so that they can be found from 
    m_Shader.load("/home/ethan/GitHub/Bread-Physics/demo/shaders/shader.vert", "/home/ethan/GitHub/Bread-Physics/demo/shaders/shader.frag");

    m_Camera.init(bMath::float3(0,0,10), 45.0f, 1.0f, 0.01f, 50.0f);

    object.init(&vertexBuffer, &m_Shader, bMath::float3(0.5f, 0.1f, 1.0f), bMath::translationMatrix(bMath::float3(0.1f,0.5f,0.2f)));
    object2.init(&vertexBuffer, &m_Shader, bMath::float3(1.0f, 0.3f, 0.2f), bMath::translationMatrix(bMath::float3(-0.1f,-0.3f,0.0f)));

}

void SceneView::render()
{
    m_Shader.use();

    frameBuffer.bind();
    
    // vertexBuffer.draw();
    object.draw();
    object2.draw();
    
    frameBuffer.unbind();
    
    // imgui code
    ImGui::Begin("Scene");
    
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    m_Size = { viewportPanelSize.x, viewportPanelSize.y };

    m_Camera.setAspect(m_Size.x / m_Size.y);
    m_Camera.updateShader(&m_Shader);
    
    // add rendered texture to ImGUI scene window
    uint64_t textureID = frameBuffer.get_texture();
    ImGui::Image((ImTextureID)(textureID), ImVec2{ m_Size.x, m_Size.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    
    ImGui::End();
}

void SceneView::onScroll(double delta) {
    m_Camera.onMouseWheel(delta);
}

void SceneView::onMove(double x, double y)
{
    m_Camera.onMouseMove(x, y);
}

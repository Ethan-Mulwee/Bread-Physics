#include "scene_view.hpp"
#include <vector>

void SceneView::init(int32_t width, int32_t height)
{
    frameBuffer.createBuffers(width,height);

    using namespace bMath;

    // quaternion rotation(34, float3(0.5f,0.3f,0.0f));

    std::vector<Vertex> testVertices = {
        {vector3(0,0,0), vector3(0,0,0)},
        {vector3(0,0,1), vector3(0,0,1)},
        {vector3(0,1,0), vector3(0,1,0)},
        {vector3(0,1,1), vector3(0,1,1)},
        {vector3(1,0,0), vector3(1,0,0)},
        {vector3(1,0,1), vector3(1,0,1)},
        {vector3(1,1,0), vector3(1,1,0)},
        {vector3(1,1,1), vector3(1,1,1)}
    };

    for (int i = 0; i < 8; i++) {
        testVertices[i].position -= vector3(0.5,0.5,0.5);
        // testVertices[i] = testVertices[i] * rotation;
        testVertices[i].position *= 0.35f;
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
    m_Shader.load("../demo/shaders/shader.vert", "../demo/shaders/shader.frag");

    m_Camera.init(bMath::float3(0,0,10), 45.0f, 1.0f, 0.01f, 50.0f);

    object.init(&vertexBuffer, &m_Shader, bMath::float3(0.5f, 0.1f, 1.0f), bMath::translationMatrix(bMath::float3(0.1f,0.5f,0.2f)));
    object2.init(&vertexBuffer, &m_Shader, bMath::float3(1.0f, 0.3f, 0.2f), bMath::translationMatrix(bMath::float3(-0.1f,-0.3f,0.0f)));

    // Cube1 ///////////////////////////////////////////////////////////////////////////////
    bEngine::RigidBody* body = new bEngine::RigidBody();
    body->inverseMass = 0.5f;
    body->inverseInertiaTensor = bMath::inverse(bMath::InertiaTensorCuboid(2,1,1,1));
    body->position = bMath::float3(0,2,0);
    body->orientation = bMath::quaternion(0.951,0.189,0.198,-0.146);
    body->orientation.normalize();

    bEngine::Primitive collider;
    collider.type = bEngine::PrimitiveType::Cube;
    collider.dimensions = bMath::float3(0.5,0.5,0.5);

    collider.offset = bMath::matrix4::identity();
    collider.body = body;

    physicsWorld.bodies.push_back(body);
    physicsWorld.colliders.push_back(collider);

    object.body = body;
    ///////////////////////////////////////////////////////////////////////////////////////

    // Cube2 ///////////////////////////////////////////////////////////////////////////////
    bEngine::RigidBody* body2 = new bEngine::RigidBody();
    body2->inverseMass = 0.5f;
    body2->inverseInertiaTensor = bMath::inverse(bMath::InertiaTensorCuboid(2,1,1,1));
    body2->position = bMath::float3(2,2,0);
    body2->orientation = bMath::quaternion(1,0,0,0);
    body2->angularVelocity = bMath::float3(0,0,0);

    bEngine::Primitive collider2;
    collider2.type = bEngine::PrimitiveType::Cube;
    collider2.dimensions = bMath::float3(0.5,0.5,0.5);
    collider2.offset = bMath::matrix4::identity();
    collider2.body = body2;

    physicsWorld.bodies.push_back(body2);
    physicsWorld.colliders.push_back(collider2);

    object2.body = body2;
    ///////////////////////////////////////////////////////////////////////////////////////

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
}

void SceneView::render()
{
    m_Shader.use();

    frameBuffer.bind();
    
    // vertexBuffer.draw();
    object.transform = object.body->getTransform();
    object2.transform = object2.body->getTransform();
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

    for (int i = 0; i < physicsWorld.bodies.size(); i++) {
        physicsWorld.bodies[i]->addForce(bMath::float3(0,-9.8,0)*(1.0f/physicsWorld.bodies[i]->inverseMass));
    }

    physicsWorld.step(1.0f/60.0f);
}

void SceneView::onScroll(double delta) {
    m_Camera.onMouseWheel(delta);
}

void SceneView::onMove(double x, double y)
{
    m_Camera.onMouseMove(x, y);
}

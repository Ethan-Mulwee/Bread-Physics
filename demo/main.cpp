#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>

#include "smath.hpp"
#include "smath_iostream.hpp"

#include "bphysics/world.hpp"
#include "bphysics/contacts.hpp"
#include "bphysics/interia_tensor.hpp"
#include "bphysics/helper.hpp"

#include "brl.hpp"

bool paused = false;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        paused = !paused;
    }
}

smath::vector3 cubeDimensions = {2.71f/2.0f, 0.527/2.0f, 0.901f/2.0f};

const int cubeCount = 15;
const float margin = 0.04f;
const float horizontalOffset = 0.901351 + margin;
const float verticalOffset = 0.527452;
const float groundOffset = 0.527452f / 2.0f;
const smath::quaternion rotated = {0, 0.707106781f, 0, 0.707106781f};
const smath::quaternion id = {0, 0, 0, 1};
smath::transform towerCubeTransforms[cubeCount] = {
    {
        smath::vector3{0, groundOffset, -horizontalOffset},
        id,
        smath::vector3{1, 1, 1}
    },
    {
        smath::vector3{0, groundOffset, 0},
        id,
        smath::vector3{1, 1, 1}
    },
    {
        smath::vector3{0, groundOffset, horizontalOffset},
        id,
        smath::vector3{1, 1, 1}
    },
    {
        smath::vector3{horizontalOffset, groundOffset + verticalOffset + margin, 0},
        rotated,
        smath::vector3{1, 1, 1}
    },
    {
        smath::vector3{0, groundOffset + verticalOffset + margin, 0},
        rotated,
        smath::vector3{1, 1, 1}
    },
    {
        smath::vector3{-horizontalOffset, groundOffset + verticalOffset + margin, 0},
        rotated,
        smath::vector3{1, 1, 1}
    },
    {
        smath::vector3{0, groundOffset + verticalOffset * 2.0f + margin, -horizontalOffset},
        smath::quaternion{0, 0, 0, 1},
        smath::vector3{1, 1, 1}
    },
    {
        smath::vector3{0, groundOffset + verticalOffset * 2.0f + margin, 0},
        smath::quaternion{0, 0, 0, 1},
        smath::vector3{1, 1, 1}
    },
    {
        smath::vector3{0, groundOffset + verticalOffset * 2.0f + margin, horizontalOffset},
        smath::quaternion{0, 0, 0, 1},
        smath::vector3{1, 1, 1}
    },
    {
        smath::vector3{horizontalOffset, groundOffset + verticalOffset * 3.0f + margin, 0},
        rotated,
        smath::vector3{1, 1, 1}
    },
    {
        smath::vector3{0, groundOffset+ verticalOffset * 3.0f + margin, 0},
        rotated,
        smath::vector3{1, 1, 1}
    },
    {
        smath::vector3{-horizontalOffset, groundOffset + verticalOffset * 3.0f + margin, 0},
        rotated,
        smath::vector3{1, 1, 1}
    },
    {
        smath::vector3{0, groundOffset + verticalOffset * 4.0f + margin, -horizontalOffset},
        smath::quaternion{0, 0, 0, 1},
        smath::vector3{1, 1, 1}
    },
    {
        smath::vector3{0, groundOffset + verticalOffset * 4.0f + margin, 0},
        smath::quaternion{0, 0, 0, 1},
        smath::vector3{1, 1, 1}
    },
    {
        smath::vector3{0, groundOffset + verticalOffset * 4.0f + margin, horizontalOffset},
        smath::quaternion{0, 0, 0, 1},
        smath::vector3{1, 1, 1}
    },
};


int main() {
    brl::Window* window = brl::createWindow(1920, 1080, "test");
    glfwSetKeyCallback(window->glfwWindow, keyCallback);
    brl::RenderContext renderContext = brl::createRenderContext(window);
    brl::ViewportContext viewport = brl::createViewportContext(&renderContext, "viewport");
    brl::Camera camera = brl::createCamera({0.0f,0.0f,0.0f}, 5.0f, 45.0f, 0.1f, 100.0f, -M_PI/4.0f, M_PI/4.0f);
    brl::Mesh roundedCubeMesh = brl::importObj("../demo/OBJs/Rounded-Cube.obj");

    bpe::World physicsWorld;

    for (int i = 0; i < cubeCount; i++) {
        smath::transform& transform = towerCubeTransforms[i];
        bpe::RigidBody* body = bpe::createRigidBody(
            transform.translation, 
            transform.rotation, 
            0.5f, 
            bpe::InertiaTensorCuboid(2,cubeDimensions.x*2.0f,cubeDimensions.y*2.0f,cubeDimensions.z*2.0f)
        );

        bpe::Primitive collider = bpe::createCollider(
            bpe::PrimitiveType::Cube, 
            cubeDimensions, 
            smath::matrix4x4_from_identity(), 
            body
        );

        physicsWorld.bodies.push_back(body);
        physicsWorld.colliders.push_back(collider);  
    }

    double physicsStepTime = 0.0;
    
    while (!brl::windowShouldClose(window)) {
        brl::updateWindow(window);
        if (viewport.hovered) brl::updateCamera(&camera, window);

        double mouseX, mouseY; 
        int screenWidth, screenHeight;
        glfwGetCursorPos(window->glfwWindow, &mouseX, &mouseY);
        glfwGetWindowSize(window->glfwWindow, &screenWidth, &screenHeight);
        screenWidth = viewport.size.x;
        screenHeight = viewport.size.y;
        mouseX -= viewport.screenPosition.x;
        mouseY -= viewport.screenPosition.y;
        mouseY = screenHeight - mouseY;


        smath::vector4 lRayStart_NDC{
            ((float)mouseX/(float)screenWidth  - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
            ((float)mouseY/(float)screenHeight - 0.5f) * 2.0f, // [0, 768] -> [-1,1]
            -1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
            1.0f
        };

        smath::vector4 lRayEnd_NDC{
            ((float)mouseX/(float)screenWidth  - 0.5f) * 2.0f,
            ((float)mouseY/(float)screenHeight - 0.5f) * 2.0f,
            0.0,
            1.0f
        };

        smath::matrix4x4 projectionMatrix = calculateCameraProjection(camera);
        smath::matrix4x4 inverseProjectionMatrix = smath::inverse(projectionMatrix);

        smath::matrix4x4 viewMatrix = calculateCameraView(camera);
        smath::matrix4x4 inverseViewMatrix = smath::inverse(viewMatrix);

        smath::vector4 lRayStart_camera = smath::matrix4x4_transform_vector4(inverseProjectionMatrix , lRayStart_NDC);    lRayStart_camera *= 1.0f/lRayStart_camera.w;
        smath::vector4 lRayStart_world  = smath::matrix4x4_transform_vector4(inverseViewMatrix       , lRayStart_camera); lRayStart_world  *= 1.0f/lRayStart_world .w;
        smath::vector4 lRayEnd_camera   = smath::matrix4x4_transform_vector4(inverseProjectionMatrix , lRayEnd_NDC);      lRayEnd_camera   *= 1.0f/lRayEnd_camera  .w;
        smath::vector4 lRayEnd_world    = smath::matrix4x4_transform_vector4(inverseViewMatrix       , lRayEnd_camera);   lRayEnd_world    *= 1.0f/lRayEnd_world   .w;

        smath::vector3 lRayDir_world = smath::vector3_from_vector4(lRayEnd_world - lRayStart_world);
        lRayDir_world = smath::normalize(lRayDir_world);

        bpe::RaycastResult raycastResult = physicsWorld.raycast(smath::vector3_from_vector4(lRayStart_world), lRayDir_world);

        brl::beginRender(window);
            brl::clearRender();

            // ImGui functions can be called here
            ImGui::Begin("Stats");
            ImGui::Text("Delta time: %fms", window->deltaTime*1000.0);
            ImGui::Text("Physics Step time: %fms", physicsStepTime*1000.0);
            #ifdef BPHYSICS_DEBUG
            ImGui::Text("Contact Generation time: %fms", physicsWorld.contactGenerationTime.count());
            ImGui::Text("Contact Resolution time: %fms", physicsWorld.contactResolutionTime.count());
            ImGui::Text("Inegration time: %fms", physicsWorld.integrationTime.count());
            #endif
            ImGui::End();

            brl::beginViewport(viewport, camera);

                // brl::drawMesh(renderContext, roundedCubeMesh, redCubeBody->getTransform()*cubeTransform, smath::vector4{1.000f,0.200f,0.322f,1.0f});
                // brl::drawMesh(renderContext, roundedCubeMesh, blueCubeBody->getTransform()*cubeTransform, smath::vector4{0.157f,0.565f,1.0f,1.0f});
                // NOTE: It seems like the orange cube's velocity isn't being resolved properly by the contact look into this
                // NOTE: the angular velocity also dones't seem to stay the rotation disappears as soon as the cube is not in contact which maybe is a result of resolvePosition rotating it rather than there being a true impluse
                // brl::drawMesh(renderContext, roundedCubeMesh, orangeCubeBody->getTransform()*cubeTransform, smath::vector4{1.0f,0.4f,0.2f,1.0f});
                if (raycastResult.hit) {
                    brl::drawSphere(renderContext, raycastResult.position, 0.03f, smath::vector4{1.0f, 0.0f, 0.0f, 1.0f});
                }

                for (int i = 0; i < physicsWorld.colliders.size(); i++) {
                    const bpe::Primitive& collider = physicsWorld.colliders[i];
                    const bpe::RigidBody* body = collider.body;
                    smath::matrix4x4 cubeTransform = smath::matrix4x4_from_transform({
                        .translation = smath::vector3{0.0f,0.0f,0.0f},
                        .rotation = smath::quaternion{0,0,0,1.0f},
                        .scale = collider.dimensions
                    });
                    brl::drawCube(renderContext, body->getTransform()*cubeTransform);
                }

                bpe::ContactPool contacts = physicsWorld.getContactPool();
                for (int i = 0; i < contacts.count(); i++) {
                    bpe::Contact& contact = contacts[i];
                    brl::drawVector(renderContext, contact.contactPoint, contact.contactNormal*0.15f, 0.04f);
                    brl::drawSphere(renderContext, contact.contactPoint, 0.06f);
                }
            brl::endViewport(viewport, camera);
        brl::endRender();

        // TODO: add caching for multiple contacts
        if (!paused) {
            for (int i = 0; i < physicsWorld.bodies.size(); i++) {
                physicsWorld.bodies[i]->addForce({0.0f, -9.81f * physicsWorld.bodies[i]->getMass(), 0.0f});
            }
            double beforePhysicsTime = glfwGetTime();
            physicsWorld.step(window->deltaTime*0.5f, 4);
            double afterPhysicsTime = glfwGetTime();
            physicsStepTime = afterPhysicsTime - beforePhysicsTime;
        }
    }

    // GLWindow* window = createWindow(1920, 1080, "window"); 
    // Renderer* renderer = new Renderer();
    // intializeRenderer(renderer, window, 1920, 1080);

    // Scene* scene = new Scene();

    // Camera camera = createCamera(smath::vector3{0.0f,0.0f,0.0f}, 5.0f, 45.0f, 0.1f, 100.0f, -M_PI/4.0f, M_PI/4.0f);
    // scene->camera = &camera;

    // glfwSetKeyCallback(window->glfwWindow, keyCallback);

    // Mesh* roundedCubeMesh = new Mesh();
    // *roundedCubeMesh = importObj("../demo/OBJs/Rounded-Cube.obj");
    // VertexBuffer roundedCubeBuffer = createVertexBuffer(roundedCubeMesh);

    // smath::transform cubeTransform = {
    //     .translation = smath::vector3{0.0f,0.0f,0.0f},
    //     .rotation = smath::quaternion{0,0,0,1.0f},
    //     .scale = smath::vector3{0.5f,0.5f,0.5f}
    // };

    // Object redCubeObject = createObject(
    //     roundedCubeBuffer, 
    //     smath::vector4{1.000f,0.200f,0.322f,1.0f},
    //     smath::matrix4x4_from_transform(cubeTransform) /* * smath::matrix4x4_from_scale(smath::vector3{2.0f,1.0f,2.0f}) */
    // );
    // bphys::RigidBody* redCubeBody = bphys::createRigidBody(
    //     smath::vector3{0,1,0}, 
    //     smath::normalized(smath::quaternion{0.1f, 0.3f, 0.6f, 1.0f}), 
    //     0.5f, 
    //     bphys::InertiaTensorCuboid(2,1,1,1)
    // );
    // bphys::Primitive redCubeCollider = bphys::createCollider(
    //     bphys::PrimitiveType::Cube, 
    //     smath::vector3{0.5f,0.5f,0.5f}, 
    //     smath::matrix4x4_from_identity(), 
    //     redCubeBody
    // );

    // addPhysicsObject(redCubeObject, redCubeBody, redCubeCollider, scene);

    // Object blueCubeObject = createObject(
    //     roundedCubeBuffer, 
    //     smath::vector4{0.157f,0.565f,1.0f,1.0f},
    //     smath::matrix4x4_from_transform(cubeTransform)
    // );
    // bphys::RigidBody* blueCubeBody = bphys::createRigidBody(
    //     smath::vector3{0,3.2,0}, 
    //     smath::normalized(smath::quaternion{0.2f, 0.1f, 0.6f, 0.2f}), 
    //     0.5f, 
    //     bphys::InertiaTensorCuboid(2,1,1,1)
    // );
    // bphys::Primitive blueCubeCollider = bphys::createCollider(
    //     bphys::PrimitiveType::Cube, 
    //     smath::vector3{0.5f,0.5f,0.5f}, 
    //     smath::matrix4x4_from_identity(),
    //     blueCubeBody
    // );

    // addPhysicsObject(blueCubeObject, blueCubeBody, blueCubeCollider, scene);

    // Object orangeCubeObject = createObject(
    //     roundedCubeBuffer,
    //     smath::vector4{1.0f,0.385f,0.136f,1.0f},
    //     smath::matrix4x4_from_transform(cubeTransform)
    // );
    // bphys::RigidBody* orangeCubeBody = bphys::createRigidBody(
    //     smath::vector3{0.5f,4.3f,0.0f}, 
    //     smath::normalized(smath::quaternion{0.6f, 0.3f, 0.2f, 0.2f}), 
    //     0.5f, 
    //     bphys::InertiaTensorCuboid(2,1,1,1)
    // );
    // bphys::Primitive orangeCubeCollider = bphys::createCollider(
    //     bphys::PrimitiveType::Cube,
    //     smath::vector3{0.5f,0.5f,0.5f},
    //     smath::matrix4x4_from_identity(),
    //     orangeCubeBody
    // );

    // addPhysicsObject(orangeCubeObject, orangeCubeBody, orangeCubeCollider, scene);


    // while(!glfwWindowShouldClose(window->glfwWindow)) { 
    //     updateWindow(window);

    //     // support for middle mouse emulation by holding alt and left click
    //     bool middleMouse = 
    //     glfwGetMouseButton(window->glfwWindow, GLFW_MOUSE_BUTTON_MIDDLE) || 
    //     (glfwGetMouseButton(window->glfwWindow, GLFW_MOUSE_BUTTON_LEFT) && glfwGetKey(window->glfwWindow, GLFW_KEY_LEFT_ALT));

    //     if (middleMouse && !glfwGetKey(window->glfwWindow, GLFW_KEY_LEFT_SHIFT)) {
    //         camera.yaw -= window->deltaMousePos.x*0.0075f;
    //         camera.pitch -= window->deltaMousePos.y*0.0075f;
    //     }   
        
    //     if (middleMouse && glfwGetKey(window->glfwWindow, GLFW_KEY_LEFT_SHIFT)) {
    //         smath::vector3 movement = smath::vector3{-window->deltaMousePos.x,window->deltaMousePos.y,0.0f};
    //         smath::quaternion cameraOrientation = calculateCameraOrientation(camera);

    //         movement = smath::quaternion_transform_vector(cameraOrientation, movement);
    //         movement *= 0.00075f;

    //         camera.focus -= movement*camera.distance;
    //     }   

    //     camera.distance -= window->scrollInput*camera.distance*0.075f;


    //     for (int i = 0; i < scene->physicsWorld.bodies.size(); i++) {
    //       scene->physicsWorld.bodies[i]->addForce(smath::vector3{0,-9.8f,0}*(1.0f/scene->physicsWorld.bodies[i]->inverseMass));
    //     }

    //     double beforePhysicsTime = glfwGetTime();
    //     if (!(paused)) {
    //         scene->physicsWorld.step(window->deltaTime, 5);
    //     }
    //     double afterPhysicsTime = glfwGetTime();

    //     bphys::ContactPool contacts = scene->physicsWorld.getContactPool();
    //     for (int i = 0; i < contacts.count(); i++) {
    //         smath::vector3 position = contacts[i].contactPoint;
    //         smath::vector3 normal = contacts[i].contactNormal;
    //         smath::vector4 color = {1.0f,1.0f,1.0f,1.0f};

    //         if (contacts[i].type == bphys::ContactType::SAT_VERT_FACE) {
    //             color = smath::vector4{1.0f,0.0f,0.0f,1.0f};
    //         } 
    //         if (contacts[i].type == bphys::ContactType::SAT_EDGE_EDGE) {
    //             color = smath::vector4{0.0f,0.0f,1.0f,1.0f};
    //         }
    //         DrawCommandSphere(renderer, smath::vector3{position.x,position.y,position.z}, 0.1f);
    //         DrawCommandVector(renderer, smath::vector3{position.x,position.y,position.z}, smath::vector3{normal.x,normal.y,normal.z}, 0.3f, 0.05f, color);
    //     }

    //     for (int i = 0; i < scene->physicsWorld.colliders.size(); i++) {
    //         switch(scene->physicsWorld.colliders[i].type) {
    //             case bphys::PrimitiveType::Cube:
    //                 DrawCommandCubeWireframe(renderer, scene->physicsWorld.colliders[i].dimensions, scene->physicsWorld.colliders[i].getTransform());
    //                 break;
    //             default:
    //                 break;

    //         }
    //     }

    //     {
    //         // adapted from https://www.opengl-tutorial.org/miscellaneous/clicking-on-objects/picking-with-a-physics-library/
    //         double mouseX, mouseY; 
    //         int screenWidth, screenHeight;
    //         glfwGetCursorPos(window->glfwWindow, &mouseX, &mouseY);
    //         glfwGetWindowSize(window->glfwWindow, &screenWidth, &screenHeight);
    //         screenWidth = scene->frameSize.x;
    //         screenHeight = scene->frameSize.y;
    //         mouseX -= scene->framePosition.x;
    //         mouseY -= scene->framePosition.y;
    //         mouseY = screenHeight - mouseY;


    //         smath::vector4 lRayStart_NDC{
    //             ((float)mouseX/(float)screenWidth  - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
    //             ((float)mouseY/(float)screenHeight - 0.5f) * 2.0f, // [0, 768] -> [-1,1]
    //             -1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
    //             1.0f
    //         };

    //         smath::vector4 lRayEnd_NDC{
    //             ((float)mouseX/(float)screenWidth  - 0.5f) * 2.0f,
    //             ((float)mouseY/(float)screenHeight - 0.5f) * 2.0f,
    //             0.0,
    //             1.0f
    //         };

    //         smath::matrix4x4 projectionMatrix = calculateCameraProjection(camera);
    //         smath::matrix4x4 inverseProjectionMatrix = smath::inverse(projectionMatrix);

    //         smath::matrix4x4 viewMatrix = calculateCameraView(camera);
    //         smath::matrix4x4 inverseViewMatrix = smath::inverse(viewMatrix);

    //         smath::vector4 lRayStart_camera = smath::matrix4x4_transform_vector4(inverseProjectionMatrix , lRayStart_NDC);    lRayStart_camera *= 1.0f/lRayStart_camera.w;
    //         smath::vector4 lRayStart_world  = smath::matrix4x4_transform_vector4(inverseViewMatrix       , lRayStart_camera); lRayStart_world  *= 1.0f/lRayStart_world .w;
    //         smath::vector4 lRayEnd_camera   = smath::matrix4x4_transform_vector4(inverseProjectionMatrix , lRayEnd_NDC);      lRayEnd_camera   *= 1.0f/lRayEnd_camera  .w;
    //         smath::vector4 lRayEnd_world    = smath::matrix4x4_transform_vector4(inverseViewMatrix       , lRayEnd_camera);   lRayEnd_world    *= 1.0f/lRayEnd_world   .w;

    //         smath::vector3 lRayDir_world = smath::vector3_from_vector4(lRayEnd_world - lRayStart_world);
    //         lRayDir_world = smath::normalized(lRayDir_world);

    //         bphys::RaycastResult raycastResult = scene->physicsWorld.raycast(smath::vector3_from_vector4(lRayStart_world), lRayDir_world);
    //         if (raycastResult.hit) {
    //             DrawCommandSphere(renderer, raycastResult.position, 0.3f);
    //         }
    //     }



    //     double beforeTime = glfwGetTime();
    //     render(renderer, scene);
    //     double afterTime = glfwGetTime();
        
    //     window->perviousRenderTime = afterTime - beforeTime;
    //     window->perviousPhysicsTime = afterPhysicsTime - beforePhysicsTime;
    // }

    // destroyWindow(window);
}
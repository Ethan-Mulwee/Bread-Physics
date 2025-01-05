#include "bEngine/render.hpp"
#include <bMath/ext/raylib.hpp>
#include <bMath/ext/iostream.hpp>

Renderer::Renderer() {
  cubeModel = LoadModelFromMesh(GenMeshCube(1,1,1)); 
}

void Renderer::render(const bEngine::World &world)
{
  for (int i = 0; i < world.bodies.size(); i++) {
    switch (world.bodies[i].collider.type) {
      case bEngine::PrimitiveType::Cube:
        cubeModel.transform = toRay(world.bodies[i].getTransform()*world.bodies[i].collider.offset);
        DrawModel(cubeModel, Vector3{0,0,0}, 1, WHITE);
        break;
    }
  }
}
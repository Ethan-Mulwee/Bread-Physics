#include "render.hpp"
#include <bMath/ext/raylib.hpp>
#include <bMath/ext/iostream.hpp>

Renderer::Renderer() {
  cubeModel = LoadModelFromMesh(GenMeshCube(1,1,1)); 
}

void Renderer::render(const bEngine::World &world)
{
  for (int i = 0; i < world.colliders.size(); i++) {
    switch (world.colliders[i]->type) {
      case bEngine::PrimitiveType::Cube:
        cubeModel.transform = toRay(world.colliders[i]->getTransform()*world.colliders[i]->offset);
        DrawModel(cubeModel, Vector3{0,0,0}, 1, WHITE);
        break;
    }
  }
}
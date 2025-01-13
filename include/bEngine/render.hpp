#ifndef BENGINE_RENDER
#define BENGINE_RENDER

#include "world.hpp"
#include <raylib.h>

class Renderer {
  public:
    Renderer();

    void render(const bEngine::World &world);

  public:
    Model cubeModel;
};

#endif
#ifndef BENGINE_VERTEXBUFFER
#define BENGINE_VERTEXBUFFER

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "bMath.hpp"

#include <vector>

class VertexBuffer {
    public:

        void createBuffers(std::vector<bMath::vector3> vertices, std::vector<uint32_t> indices);

        void delete_buffers();

        void bind();

        void unbind();

        void draw();

    protected:
        std::vector<bMath::vector3> m_Vertices; std::vector<uint32_t> m_Indices;
        uint32_t vao, vbo, ebo;

};

#endif
#ifndef BENGINE_VERTEXBUFFER
#define BENGINE_VERTEXBUFFER

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "bMath.hpp"

#include <vector>

struct Vertex {
    bMath::float3 position;
    bMath::float3 normal;
};

class VertexBuffer {
    public:

        void createBuffers(std::vector<Vertex> vertices, std::vector<uint32_t> indices);

        void delete_buffers();

        void bind();

        void unbind();

        void draw();

    protected:
        std::vector<Vertex> m_Vertices; 
        std::vector<uint32_t> m_Indices;
        uint32_t vao, vbo, ebo;

};

#endif
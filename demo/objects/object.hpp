#ifndef BENGINE_OBJECT
#define BENGINE_OBJECT

#include "render/vertexbuffer.hpp"

#include "shader/b_shader.hpp"

#include "bMath.hpp"

class Object {
    public:
        void init(VertexBuffer* _vertexBuffer, Shader* _shader, bMath::float3 _color, bMath::matrix4 _transform);

        void draw();

    public:
        VertexBuffer* vertexBuffer;
        Shader* shader;

        bMath::float3 color;
        bMath::matrix4 transform;

};

#endif
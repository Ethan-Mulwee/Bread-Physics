#include "object.hpp"

void Object::init(VertexBuffer* _vertexBuffer, Shader* _shader, bMath::float3 _color, bMath::matrix4 _transform) {
    vertexBuffer = _vertexBuffer;
    shader = _shader;
    color = _color;
    transform = _transform;
}

void Object::draw() {
    shader->use();

    shader->setMatrix4(transform, "model");
    bMath::float4 color4(color.x,color.y,color.z,1.0f);
    shader->setFloat4(color4, "color");

    vertexBuffer->draw();
}
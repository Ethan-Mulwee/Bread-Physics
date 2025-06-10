#version 330 core

layout(location = 0) in vec4 position;
out vec3 v_vertexPosition;

void main()
{
    v_vertexPosition = vec3(position);
    gl_Position = position;
};
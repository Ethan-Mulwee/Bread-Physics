#version 330 core

in vec3 v_vertexPosition;
layout(location = 0) out vec4 color;

void main()
{
    color = vec4((v_vertexPosition.x*0.5)+0.5, (v_vertexPosition.y*0.5)+0.5, (v_vertexPosition.z*0.5)+0.5, 1.0);
};
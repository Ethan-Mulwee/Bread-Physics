#version 330 core

in vec4 Color;
in vec3 Normal;
layout(location = 0) out vec4 color;

void main()
{
    // color = Color;
    color = vec4(Normal, 1.0f);
};
#version 330 core

in vec4 Color;
in vec3 Normal;
layout(location = 0) out vec4 color;

void main()
{
    // color = Color;
    // color = vec4(Normal, 1.0f);
    float light = dot(Normal, normalize(vec3(1.0,1.0,1.0)));
    light = clamp(light, 0.0, 1.0);
    light += 0.1;

    float light2 = dot(Normal, normalize(vec3(-1.3,-0.2,-1.0)));
    light2 = clamp(light2, 0.0, 1.0);
    light2 *= 0.2;

    light += light2;

    color = vec4(vec3(light), 1.0f);
};
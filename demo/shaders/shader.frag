#version 330 core

in vec4 Color;
in vec3 Normal;
in vec3 WorldPos;
layout(location = 0) out vec4 color;

uniform mat4 model;

void main()
{
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 normal = normalize(normalMatrix * Normal);


    float light = dot(normal, normalize(vec3(1.0,1.0,1.0)));
    light = clamp(light, 0.0, 1.0);
    light += 0.1;

    float light2 = dot(normal, normalize(vec3(-1.3,-0.2,-1.0)));
    light2 = clamp(light2, 0.0, 1.0);
    light2 *= 0.2;

    light += light2;


    color = vec4(vec3(Color)*light, 1.0f);
};
#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

struct MVPMatrices {
    mat4 model;
    mat4 view;
    mat4 projection;
};

uniform MVPMatrices modelViewProjection;

uniform float outlineScale = 1.05;

void main()
{
    vec3 scaledPos = aPos * outlineScale;
    gl_Position = modelViewProjection.projection * modelViewProjection.view * modelViewProjection.model * vec4(scaledPos, 1.0);
}
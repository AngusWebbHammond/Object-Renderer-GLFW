#version 450 core

layout (location = 0) in vec3 aPos;

struct MVPMatrices {
    mat4 model;
    mat4 view;
    mat4 projection;
};

uniform MVPMatrices modelViewProjection;

void main()
{
    gl_Position = modelViewProjection.projection * modelViewProjection.view * modelViewProjection.model * vec4(aPos, 1.0);
}

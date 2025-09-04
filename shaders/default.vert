#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aTexture;

out vec3 normal;
out vec3 fragPos;

struct MVPMatrices {
    mat4 model;
    mat4 view;
    mat4 projection;
};

uniform MVPMatrices modelViewProjection;

void main()
{
    gl_Position = modelViewProjection.projection * modelViewProjection.view * modelViewProjection.model * vec4(aPos, 1.0f);

    fragPos = vec3(modelViewProjection.model * vec4(aPos, 1.0f));

    mat3 normalMatrix = transpose(inverse(mat3(modelViewProjection.model)));
    normal = normalize(normalMatrix * aNormal);
}
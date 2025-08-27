#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aTexture;

out vec3 vertexColour;
out vec3 normal;
out vec3 lightingPos;
out vec3 lightingCol;
out float lightingInt;
out vec3 fragPos;

uniform vec3 lightingPosition;
uniform vec3 lightingColour;
uniform float lightingIntensity;
uniform vec3 meshColour;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out mat3 outModel;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    fragPos = vec3(model * vec4(aPos, 1.0f));
    vertexColour = meshColour;
    normal = aNormal;
    lightingPos = lightingPosition;
    lightingCol = lightingColour;
    lightingInt = lightingIntensity;

    outModel = mat3(model);
}
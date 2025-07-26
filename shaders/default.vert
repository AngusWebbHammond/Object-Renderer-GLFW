#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aTexture;

out vec3 vertexColour;
out vec3 normal;
out vec3 lightingPos;

uniform mat4 transform;
uniform vec3 lightingPosition;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    vertexColour = aColour;
    normal = aNormal;
    lightingPos = lightingPosition;
}
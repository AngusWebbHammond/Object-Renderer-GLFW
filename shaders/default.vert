#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 0) in vec3 aColour;
out vec3 vertexColour;
out vec3 glFragCoord;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0f);
    vertexColour = aColour;
    glFragCoord = aPos;
}
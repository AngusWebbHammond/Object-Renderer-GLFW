#version 330 core

out vec4 FragColour;
in vec3 vertexColour;
in vec3 glFragCoord;

void main()
{
    FragColour = vec4(glFragCoord.zxy, 1.0f);
    // FragColour = vec4(vertexColour, 1.0f);
}   
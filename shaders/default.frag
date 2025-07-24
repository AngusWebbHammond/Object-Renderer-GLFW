#version 450 core

out vec4 FragColour;
in vec3 vertexColour;
in vec3 glFragCoord;

void main()
{
    FragColour = vec4(result, 1.0f);
}   

//TODO Add lighting to this shader, using the normals loaded in from the .obj file
//TODO Add textures to this shader, including specific colours for each face
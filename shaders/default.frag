#version 450 core

out vec4 FragColour;

in vec3 vertexColour;
in vec3 glFragCoord;
in vec3 normal;
in vec3 lightingPos;
in vec3 lightingCol;
in float lightingInt;
in vec3 fragPos;
in mat3 outModel;

void main()
{
    float ambientStrength = 0.2;
    vec3 normalizedNormal = normalize(outModel * normal);
    vec3 lightingDirection = normalize(lightingPos - fragPos);
    float diff = max(dot(normalizedNormal, lightingDirection), 0.0);
    vec3 diffuse = diff * lightingCol;
    vec3 ambient = ambientStrength * lightingCol;
    vec3 result = lightingInt * (diffuse + ambient) * vertexColour;

    FragColour = vec4(result, 1.0f);
}

//TODO Add textures to this shader, including specific colours for each face
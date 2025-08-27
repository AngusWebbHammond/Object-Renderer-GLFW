#version 450 core

out vec4 FragColour;

in vec3 vertexColour;
in vec3 normal;
in vec3 lightingPos;
in vec3 lightingCol;
in float lightingInt;
in vec3 fragPos;
in mat3 outModel;

in vec3 viewPosition;

void main()
{
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightingCol;

    vec3 normalizedNormal = normalize(outModel * normal);
    vec3 lightingDirection = normalize(lightingPos - fragPos);
    float diff = max(dot(normalizedNormal, lightingDirection), 0.0);
    vec3 diffuse = diff * lightingCol;

    float specularStrength = 0.5;
    vec3 viewDirection = normalize(viewPosition - fragPos);
    vec3 reflectDirection = reflect(-lightingDirection, normalizedNormal);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
    vec3 specular = specularStrength * spec * lightingCol;  

    vec3 result = lightingInt * (specular + diffuse + ambient) * vertexColour;

    FragColour = vec4(result, 1.0f);
}

//TODO Add textures to this shader, including specific colours for each face
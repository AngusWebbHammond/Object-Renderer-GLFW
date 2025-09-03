#version 450 core

out vec4 FragColour;

in vec3 normal;
in vec3 fragPos;

uniform float lightingIntensity;

uniform vec3 lightingPosition;
uniform vec3 lightingColour;
uniform vec3 viewPos;
uniform vec3 meshColour;

uniform mat4 model;

void main()
{
    // Ambient Lighting
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightingColour;

    // Diffuse Lighting
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 normalizedNormal = normalize(normalMatrix * normal);
    vec3 lightingDirection = normalize(lightingPosition - fragPos);
    float diff = max(dot(normalizedNormal, lightingDirection), 0.0);
    vec3 diffuse = diff * lightingColour;

    // Specular Lighting
    float specularStrength = 1.0;
    vec3 viewDirection = normalize(viewPos - fragPos);
    vec3 reflectDirection = reflect(-lightingDirection, normalizedNormal);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 256);
    vec3 specular = specularStrength * spec * lightingColour;

    // Resulting Lighting
    vec3 result = lightingIntensity * (specular + diffuse + ambient) * meshColour;
    FragColour = vec4(result, 1.0f);
}

//TODO Add textures to this shader, including specific colours for each face
#version 450 core

out vec4 FragColour;

in vec3 normal;
in vec3 fragPos;

struct PointLight {
    vec3 position;
    vec3 colour;
    float intensity;

    float ambient;
    float specular;
    float diffuse;
};

uniform vec3 viewPos;
uniform vec3 meshColour;

uniform PointLight light;

void main()
{
    // Ambient Lighting
    vec3 ambient = light.ambient * light.colour;

    // Diffuse Lighting
    vec3 normalizedNormal = normalize(normal);
    vec3 lightingDirection = normalize(light.position - fragPos);
    float diff = max(dot(normalizedNormal, lightingDirection), 0.0);
    vec3 diffuse = light.diffuse * diff * light.colour;

    // Specular Lighting
    vec3 viewDirection = normalize(viewPos - fragPos);
    vec3 reflectDirection = reflect(-lightingDirection, normalizedNormal);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 256);
    vec3 specular = light.specular * spec * light.colour;

    // Resulting Lighting
    vec3 result = light.intensity * (specular + diffuse + ambient) * meshColour;
    FragColour = vec4(result, 1.0f);
}

//TODO Add textures to this shader, including specific colours for each face
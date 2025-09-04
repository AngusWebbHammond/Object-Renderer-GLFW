#version 450 core

#define MAX_POINT_LIGHTS 16

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

uniform int numPointLights;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

vec3 calculatePointLight(PointLight pointLight) {
    // Ambient Lighting
    vec3 ambient = pointLight.ambient * pointLight.colour;

    // Diffuse Lighting
    vec3 normalizedNormal = normalize(normal);
    vec3 lightingDirection = normalize(pointLight.position - fragPos);
    float diff = max(dot(normalizedNormal, lightingDirection), 0.0);
    vec3 diffuse = pointLight.diffuse * diff * pointLight.colour;

    // Specular Lighting
    vec3 viewDirection = normalize(viewPos - fragPos);
    vec3 reflectDirection = reflect(-lightingDirection, normalizedNormal);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 256);
    vec3 specular = pointLight.specular * spec * pointLight.colour;

    // Resultant Contriubution of all Lighting
    return pointLight.intensity * (specular + diffuse + ambient);
}

void main()
{
    vec3 result = vec3(0.0f);

    for (int i = 0; i < numPointLights; i++) {
        result += calculatePointLight(pointLights[i]);
    }

    FragColour = vec4(result * meshColour, 1.0f);
}

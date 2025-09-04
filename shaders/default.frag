#version 450 core

#define MAX_POINT_LIGHTS 16
#define MAX_DIRECTIONAL_LIGHTS 4
#define MAX_SPOT_LIGHTS 4

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

    float constantAttenuationFactor;
    float linearAttenuationFactor;
    float quadraticAttenuationFactor;
};

struct DirectionalLight {
    vec3 direction;
    vec3 colour;

    float intensity;

    float ambient;
    float specular;
    float diffuse;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 colour;

    float intensity;

    float cutOff;
    float outerCutOff;

    float ambient;
    float specular;
    float diffuse;

    float constantAttenuationFactor;
    float linearAttenuationFactor;
    float quadraticAttenuationFactor;
};

uniform vec3 viewPos;
uniform vec3 meshColour;

uniform int numPointLights;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform int numDirectionalLights;
uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];

uniform int numSpotLights;
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

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

    float distanceFromLight = length(pointLight.position - fragPos);
    float attenuation = 1.0f / (pointLight.constantAttenuationFactor 
                                + pointLight.linearAttenuationFactor * distanceFromLight 
                                + pointLight.quadraticAttenuationFactor * (distanceFromLight * distanceFromLight));

    // Resultant Contriubution of all Lighting
    return pointLight.intensity * (specular + diffuse + ambient) * attenuation;
}

vec3 calculateDirectionalLight(DirectionalLight directionalLight) {
    // Ambient Lighting
    vec3 ambient = directionalLight.ambient * directionalLight.colour;

    // Diffuse Lighting
    vec3 normalizedNormal = normalize(normal);
    vec3 lightingDirection = normalize(directionalLight.direction);
    float diff = max(dot(normalizedNormal, lightingDirection), 0.0);
    vec3 diffuse = directionalLight.diffuse * diff * directionalLight.colour;

    // Specular Lighting
    vec3 viewDirection = normalize(viewPos - fragPos);
    vec3 reflectDirection = reflect(-lightingDirection, normalizedNormal);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 256);
    vec3 specular = directionalLight.specular * spec * directionalLight.colour;

    // Resultant Contriubution of all Lighting
    return directionalLight.intensity * (specular + diffuse + ambient);
}

vec3 calculateSpotLight(SpotLight spotLight) {
    vec3 lightDir = normalize(spotLight.position - fragPos);

    float theta = dot(lightDir, normalize(-spotLight.direction));
    
    if (theta > spotLight.cutOff) {
        vec3 ambient = spotLight.ambient * spotLight.colour;

        // Diffuse Lighting
        vec3 normalizedNormal = normalize(normal);
        vec3 lightingDirection = normalize(spotLight.position - fragPos);
        float diff = max(dot(normalizedNormal, lightingDirection), 0.0);
        vec3 diffuse = spotLight.diffuse * diff * spotLight.colour;

        // Specular Lighting
        vec3 viewDirection = normalize(viewPos - fragPos);
        vec3 reflectDirection = reflect(-lightingDirection, normalizedNormal);
        float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 256);
        vec3 specular = spotLight.specular * spec * spotLight.colour;

        float distanceFromLight = length(spotLight.position - fragPos);
        float attenuation = 1.0f / (spotLight.constantAttenuationFactor 
                                    + spotLight.linearAttenuationFactor * distanceFromLight 
                                    + spotLight.quadraticAttenuationFactor * (distanceFromLight * distanceFromLight));

        // Resultant Contriubution of all Lighting
        return spotLight.intensity * (ambient + (specular + diffuse) * attenuation);
    }
    else {
        return spotLight.intensity * spotLight.ambient * spotLight.colour;
    }
}

void main()
{
    vec3 result = vec3(0.0f);

    for (int i = 0; i < numPointLights; i++) {
        result += calculatePointLight(pointLights[i]);
    }

    for (int i = 0; i < numDirectionalLights; i++) {
        result += calculateDirectionalLight(directionalLights[i]);
    }

    for (int i = 0; i < numSpotLights; i++) {
        result += calculateSpotLight(spotLights[i]);
    }

    FragColour = vec4(result * meshColour, 1.0f);
}

// TODO update this shader to include materials and other type of lights
#version 450 core

out vec4 FragColour;

struct PointLight {
    vec3 colour;
    float intensity;
};

uniform PointLight light;

void main()
{
    FragColour = light.intensity * vec4(light.colour, 1.0);
}
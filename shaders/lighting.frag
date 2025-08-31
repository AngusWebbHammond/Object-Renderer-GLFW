#version 450 core

out vec4 FragColour;

uniform vec3 lightingColour;
uniform float lightingIntensity;

void main()
{
    FragColour = lightingIntensity * vec4(lightingColour, 1.0);
}
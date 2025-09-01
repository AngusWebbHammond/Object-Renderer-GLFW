#version 450 core
out vec4 FragColor;

uniform vec3 outlineColor = vec3(1.0, 1.0, 0.0);

void main()
{
    FragColor = vec4(outlineColor, 1.0);
}

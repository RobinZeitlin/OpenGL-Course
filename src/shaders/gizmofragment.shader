#version 330 core
out vec4 FragColor;

in vec2 texCord;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 objectColor;
uniform float alpha;

void main()
{
    FragColor = vec4(objectColor, alpha);
}

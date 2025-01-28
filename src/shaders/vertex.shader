#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCord;
layout (location = 2) in vec3 ANormCord;

out vec4 vertexColor;
out vec2 texCord;
out vec3 nCord;

uniform mat4 transform;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * transform * vec4(aPos, 1.0);
    vertexColor = vec4(0.0, 0.0, 0.0, 0.2);
    nCord = ANormCord;
    texCord = aTexCord;
}
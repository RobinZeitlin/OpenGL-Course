#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCord;

out vec4 vertexColor;
out vec2 texCoord;

uniform mat4 transform;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * transform * vec4(aPos, 1.0);
    vertexColor = vec4(1.0, 0.0, 0.0, 1.0);
    texCoord = aTexCord;
}
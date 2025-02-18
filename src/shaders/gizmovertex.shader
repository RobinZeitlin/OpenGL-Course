#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCord;
layout (location = 2) in vec3 aNormCord;

out vec2 texCord;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 transform;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * transform * vec4(aPos, 1.0);
    FragPos = vec3(transform * vec4(aPos, 1.0));
    Normal = aNormCord;
    texCord = aTexCord;
}

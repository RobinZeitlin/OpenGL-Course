#version 330 core
out vec4 FragColor;

in vec2 texCord;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D _texture;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

uniform int shaderEnabled;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = 0.15f + diff * lightColor;
    
    vec3 texColor = texture(_texture, texCord).rgb;
    
    vec3 result = texColor * diffuse;
    FragColor = vec4(result, 1.0);
}

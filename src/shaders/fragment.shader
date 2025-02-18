#version 330 core
out vec4 FragColor;

in vec2 texCord;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D _texture;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

uniform int lightType;
uniform vec3 lightDir;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDirNormalized = normalize(lightPos - FragPos);
    float diff = 0.0;
    
    // directional light
    if (lightType == 0)
    {
        lightDirNormalized = normalize(lightDir);
        diff = max(dot(norm, -lightDirNormalized), 0.0);
    }
    
    // point light
    else if(lightType == 1)
    {
        lightDirNormalized = normalize(lightPos - FragPos);
        diff = max(dot(norm, lightDirNormalized), 0.0);
    }
    
    // spotlight
    else if (lightType == 2)
    {
        lightDirNormalized = normalize(lightPos - FragPos);
        
        float theta = dot(normalize(lightDir), normalize(FragPos - lightPos));
        float intensity = max(dot(norm, lightDirNormalized), 0.0) * theta;
        diff = intensity;
    }

    vec3 diffuse = 0.15f + diff * lightColor;
    vec3 texColor = texture(_texture, texCord).rgb;

    vec3 result = texColor * diffuse;
    FragColor = vec4(result, 1.0);
}

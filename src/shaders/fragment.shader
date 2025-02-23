#version 330 core
out vec4 FragColor;

in vec2 texCord;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D _texture;
uniform vec3 lightPos[10];
uniform vec3 lightColor[10];
uniform vec3 viewPos;

uniform int lightType[10];
uniform vec3 lightDir[10];

uniform int numLights;

void main() {
    vec3 norm = normalize(Normal);
    vec3 result = vec3(0.0);

    for (int i = 0; i < numLights; i++) {
        vec3 lightDirNormalized;
        float diff = 0.0;
        
        // directional light
        if (lightType[i] == 0) {
            lightDirNormalized = normalize(lightDir[i]);
            diff = max(dot(norm, -lightDirNormalized), 0.0);
        }
        
        // point light
        else if (lightType[i] == 1) {
            lightDirNormalized = normalize(lightPos[i] - FragPos);
            diff = max(dot(norm, lightDirNormalized), 0.0);
        }
        
        // spotlight
        else if (lightType[i] == 2) {
            lightDirNormalized = normalize(lightPos[i] - FragPos);
            float theta = dot(normalize(lightDir[i]), normalize(FragPos - lightPos[i]));
            float intensity = max(dot(norm, lightDirNormalized), 0.0) * theta;
            diff = intensity;
        }

        result += diff * lightColor[i];
    }

    vec3 texColor = texture(_texture, texCord).rgb;
    vec3 finalColor = texColor * result;

    FragColor = vec4(finalColor, 1.0);
}

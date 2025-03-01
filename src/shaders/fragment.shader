#version 330 core
out vec4 FragColor;

in vec2 texCord;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D _texture;
uniform sampler2D _albedo;
uniform sampler2D _specular;

uniform vec3 lightPos[10];
uniform vec3 lightColor[10];
uniform vec3 viewPos;

uniform int lightType[10];
uniform vec3 lightDir[10];

uniform int numLights;

void main() {
    vec3 norm = normalize(Normal);
    vec3 result = vec3(0.0);
    vec3 viewDir = normalize(viewPos - FragPos);

    for (int i = 0; i < numLights; i++) {
        vec3 lightDirNormalized;
        float diff = 0.0;
        float spec = 0.0;
        
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

        vec3 halfwayDir = normalize(lightDirNormalized + viewDir);
        spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);

        vec3 lightEffect = (diff + spec) * lightColor[i];
        result += lightEffect;
    }

    vec3 baseColor = texture(_texture, texCord).rgb;
    vec3 albedoColor = texture(_albedo, texCord).rgb;
    vec3 specularColor = texture(_specular, texCord).rgb;

    vec3 finalColor = (baseColor * albedoColor) * result + specularColor;

    FragColor = vec4(finalColor, 1.0);
}

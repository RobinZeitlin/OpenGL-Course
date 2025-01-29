#version 330 core
out vec4 FragColor;

in vec2 texCord;
in vec3 nCord;

uniform sampler2D _texture;

void main()
{
    FragColor = texture(_texture, texCord);
    //FragColor = vec4((nCord).rgb, 1.0);
    //FragColor.xy = texCoord.xy;
    //FragColor = vec4(texCoord.x, texCoord.y, 0.0, 1.0);
   
}
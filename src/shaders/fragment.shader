#version 330 core
out vec4 FragColor;

in vec4 vertexColor;
in vec2 texCord;

uniform sampler2D _texture;

void main()
{
    FragColor = texture(_texture, texCord);
    //FragColor = vec4(texCoord.x, texCoord.y, 0.0, 1.0);
    /*FragColor.xy = texCoord.xy;
    FragColor.z = 0;
    
    if(FragColor.x < 0.8)
        FragColor.x = 0;*/
}
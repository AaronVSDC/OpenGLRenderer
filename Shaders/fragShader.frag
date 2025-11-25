#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D snowTexture;
uniform sampler2D containerTexture;

void main()
{ 
    FragColor = mix(texture(snowTexture, TexCoord), texture(containerTexture, TexCoord), 0.2);
}
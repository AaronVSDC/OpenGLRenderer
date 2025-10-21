#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D awesomefaceTexture;
uniform sampler2D containerTexture;

void main()
{
    FragColor = mix(texture(awesomefaceTexture, TexCoord), texture(containerTexture, TexCoord), 0.2);
}
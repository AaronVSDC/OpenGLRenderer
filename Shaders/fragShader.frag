#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;

uniform sampler2D diffuseTexture;
uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec3 ambientColor;

void main()
{
    vec4 albedo = texture(diffuseTexture, TexCoord);
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-lightDirection);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 lighting = ambientColor + (diff * lightColor);
    vec3 litColor = albedo.rgb * lighting;

    FragColor = vec4(litColor, albedo.a);
}
#version 330 core

in vec2 vsTexCoord;

out vec4 FragColor;

uniform sampler2D texture0;

void main()
{
    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f); // texture(texture0, vsTexCoord);
} 
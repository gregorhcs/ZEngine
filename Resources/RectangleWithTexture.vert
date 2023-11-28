#version 330 core

layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec2 inTexCoord;

out vec3 vsColor;
out vec2 vsTexCoord;

uniform mat4 projection;
uniform mat4 model;

void main()
{
    gl_Position = projection * model * vec4(inPosition, 0.f, 1.f);
    vsTexCoord  = inTexCoord;
}
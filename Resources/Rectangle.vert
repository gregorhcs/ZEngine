#version 330 core

layout (location = 0) in vec2 inPosition;

uniform vec4 transform;

void main()
{
    gl_Position = vec4(inPosition, 0.f, 1.f) + transform;
}
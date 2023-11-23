#version 400

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

out vec3 vsColor;
out vec2 vsTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(inPosition, 1.f);
    vsTexCoord  = inTexCoord;
}

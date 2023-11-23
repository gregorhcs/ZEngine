#version 400

in vec2 vsTexCoord;

out vec4 fsColor;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main() {
    fsColor = mix(texture(texture0, vsTexCoord), texture(texture1, vec2(1.0-vsTexCoord.x, vsTexCoord.y)), 0.2);
}

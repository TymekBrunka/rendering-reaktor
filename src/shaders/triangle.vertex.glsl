#version 150 core

in vec2 pos;
in vec3 color;

out vec3 col;
out vec2 texpos;

void main() {
    gl_Position = vec4(pos, 1.0, 1.0);
    col = color;
    texpos = (pos / 2.0) + vec2(0.5, 0.5);
}
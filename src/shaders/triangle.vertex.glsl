#version 150 core

in vec2 pos;
in vec3 color;

out vec3 col;

void main() {
    gl_Position = vec4(pos, 1.0, 1.0);
    col = color;
};
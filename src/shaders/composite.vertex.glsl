#version 330 core

in vec2 pos;
in vec2 uv;
out vec2 out_uv;

void main() {
    gl_Position = vec4(pos, 1.0, 1.0);
    out_uv = uv;
}

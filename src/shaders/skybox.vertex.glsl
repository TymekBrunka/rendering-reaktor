#version 150 core

in vec3 pos;
in vec2 uv;
out vec2 out_uv;

uniform mat4 rotat;

void main() {
    gl_Position = rotat * vec4(pos, 1.0);
    out_uv = uv;
}

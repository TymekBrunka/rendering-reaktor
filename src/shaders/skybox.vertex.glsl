#version 150 core

in vec3 pos;
in vec2 uv;
in vec3 normal;
out vec2 out_uv;

uniform mat4 world_camera;

void main() {
    gl_Position = world_camera * vec4(pos, 1.0);
    out_uv = uv;
}

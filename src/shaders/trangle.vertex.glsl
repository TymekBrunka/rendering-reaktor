#version 320

in vec2 pos;
in vec3 color;

out vce3 col;

void main() {
    gl_Position = vec4(pos, 1.0, 1.0);
    col = color;
}

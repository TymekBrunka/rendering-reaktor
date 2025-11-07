#version 150 core

in vec3 col;
in vec2 texpos;
out vec4 frag_color;

uniform sampler2D tex;

void main() {
    frag_color = texture(tex, texpos);
}
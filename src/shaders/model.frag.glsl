#version 330 core

in vec2 out_uv;
layout (location = 0) out vec4 frag_color;
layout (location = 1) out vec4 id_color;
// out vec4 id_color;

uniform sampler2D tex;
uniform vec4 id;

void main() {
    // frag_color = texture(tex, out_uv);
    // id_color = id;

    frag_color = vec4(1.0, 0.0, 0.0, 1.0);
    id_color = vec4(0.0, 1.0, 0.0, 1.0);
}

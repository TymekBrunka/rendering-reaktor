#version 330 core

in vec2 out_uv;
out vec4 frag_color;

uniform sampler2D skybox;
uniform sampler2D scene;

void main() {
    vec4 sceneColor = texture(scene, out_uv);
    vec4 skyboxColor = texture(skybox, out_uv);
    // frag_color = sceneColor.a >= 0.0 ? sceneColor : skyboxColor;
    frag_color = skyboxColor;
}

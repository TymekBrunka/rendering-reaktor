#version 150

uniform sampler2D tex_3d;
uniform sampler2D tex_ui;

in vec2 uvv;
out vec4 frag_color;

void main() {
	//frag_color = mix(texture(tex_3d, uvv), texture(tex_ui, uvv), texture(tex_ui, uvv).w);
	frag_color = vec4(1.0, 0.5, 1.0, 1.0);
}
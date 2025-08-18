#version 150

uniform sampler2D tex_3d;
uniform sampler2D tex_ui;

in vec2 uvv;
out vec4 frag_color;

#define uv (uvv + vec2(1.0)) / 2

void main() {
	frag_color = mix(texture(tex_3d, uv), texture(tex_ui, uv), texture(tex_ui, uv).w);
	// frag_color = vec4(1.0, 0.5, 1.0, 1.0);
	// frag_color = texture(tex_3d, uv);
}
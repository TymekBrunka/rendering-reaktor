#version 150

in vec2 uv;
out vec2 uvv;

void main() {
	gl_Position = vec4(uv, 1.0, 1.0);
	uvv = uv;
}
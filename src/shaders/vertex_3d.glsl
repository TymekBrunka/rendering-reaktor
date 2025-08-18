#version 150

uniform mat4 mpv;

in vec3 position;
in vec3 color;

out vec4 out_color;

void main() {
	gl_Position = mpv * vec4(position * vec3(1.0, 1.0, -1.0), 1.0);
	out_color = mix(
        vec4(0.1, 0.0, 0.05, 1.0),
        vec4(color, 1.0),
        1.0 / clamp(gl_Position.z, 10.0, 10000.0) * 10.0
        // typ
    );
}
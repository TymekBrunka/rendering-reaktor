#version 330

uniform mat4 pv_matrix;

in vec3 position;
in float color_ratio;

out vec4 color;

void main() {
    gl_Position = pv_matrix * vec4(position, 1.0);
    color = mix(
        vec4(0.1, 0.0, 0.05, 1.0),
        mix(vec4(0.3, 0.6, 0.5, 1.0), vec4(0.2, 0.4, 0.6, 1.0), color_ratio),
        1.0 / clamp(gl_Position.z, 10.0, 10000.0) * 10.0
    );
}

#version 150

uniform mat4 mpv;
uniform vec2 unit_size;

in vec3 position;
in float element_type;
in float vertex_type;
in float char_offset;
in float char_index;

flat out float typ;
out vec2 point_cord;

float dist;

void main() {
	gl_Position = mpv * vec4(position * vec3(1.0, 1.0, -1.0), 1.0);
    dist = 4.0 / sqrt(gl_Position.w);
    gl_Position.xyz /= gl_Position.w;
    gl_Position.w = 1.0;
	typ = element_type;
	if (element_type == 1.0) {
        if (vertex_type == 1.0) {
            gl_Position.xy += vec2(-0.5, 1.0) * unit_size * 20.0;
            point_cord = vec2(-1.0, 2.0);
        } else if (vertex_type == 2.0) {
            gl_Position.xy += vec2(-0.5, -0.5) * unit_size * 20.0;
            point_cord = vec2(-1.0, -1.0);
        } else {
            gl_Position.xy += vec2(1.0, -0.5) * unit_size * 20.0;
            point_cord = vec2(2.0, -1.0);
        }
    } else if (element_type == 2.0) {
        if (vertex_type == 1.0) {
            gl_Position.xy += vec2(3.5 + (3.5 * char_offset), 4.5) * unit_size * 3.5 * dist;
            point_cord = vec2(0.0 + char_index, 0.0);
        } else if (vertex_type == 2.0) {
            gl_Position.xy += vec2(3.5 + (3.5 * char_offset), -4.5) * unit_size * 3.5 * dist;
            point_cord = vec2(0.0 + char_index, 1.0);
        } else if (vertex_type == 3.0) {
            gl_Position.xy += vec2(7.0 + (3.5 * char_offset), -4.5) * unit_size * 3.5 * dist;
            point_cord = vec2((1.0 / 128.0) + char_index, 1.0);
        } else {
            gl_Position.xy += vec2(7.0 + (3.5 * char_offset), 4.5) * unit_size * 3.5 * dist;
            point_cord = vec2((1.0 / 128.0) + char_index, 0.0);
        }
    }
}
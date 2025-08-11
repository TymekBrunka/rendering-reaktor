#version 150

uniform mat4 mpv;
uniform vec2 unit_size;

in vec3 position;
in uint element_type;
in uint vertex_type;
in int char_offset;
in uint char_index;

flat out uint typ;
out vec2 point_cord;

void main() {
	gl_Position = mpv * vec4(position * vec3(1.0, 1.0, -1.0), 1.0);
	typ = vertex_type;
	if (typ == uint(1)) {
        gl_Position.xyz /= gl_Position.w;
        gl_Position.z = clamp(gl_Position.z, -5.0, -1.0);
        gl_Position.xyz /= gl_Position.w;
        gl_Position.w = 1.0;
        if (vertex_type == uint(1)) {
            gl_Position.xy += vec2(-0.5, 1.0) * unit_size * 20.0;
            point_cord = vec2(-1.0, 2.0);
        } else if (vertex_type == uint(2)) {
            gl_Position.xy += vec2(-0.5, -0.5) * unit_size * 20.0;
            point_cord = vec2(-1.0, -1.0);
        } else {
            gl_Position.xy += vec2(1.0, -0.5) * unit_size * 20.0;
            point_cord = vec2(2.0, -1.0);
        }
    } else if (typ == uint(2)) {
        gl_Position.xyz /= gl_Position.w;
        gl_Position.z = clamp(gl_Position.z, -5.0, -1.0);
     	gl_Position.xyz /= gl_Position.w;
        gl_Position.w = 1.0;
        if (vertex_type == uint(1)) {
            gl_Position.xy += vec2(3.5 + (3.5 * char_offset), 4.5) * unit_size * 3.5;
            point_cord = vec2(0.0 + ( (1.0/128.0) * float(char_index)), 0.0);
        } else if (vertex_type == uint(2)) {
            gl_Position.xy += vec2(3.5 + (3.5 * char_offset), -4.5) * unit_size * 3.5;
            point_cord = vec2(0.0 + ( (1.0/128.0) * float(char_index)), 1.0);
        } else if (vertex_type == uint(3)) {
            gl_Position.xy += vec2(7.0 + (3.5 * char_offset), -4.5) * unit_size * 3.5;
            point_cord = vec2((1.0 / 128.0) + ( (1.0/128.0) * float(char_index)), 1.0);
        } else {
            gl_Position.xy += vec2(7.0 + (3.5 * char_offset), 4.5) * unit_size * 3.5;
            point_cord = vec2((1.0 / 128.0) + ( (1.0/128.0) * float(char_index)), 0.0);
        }
    }
}
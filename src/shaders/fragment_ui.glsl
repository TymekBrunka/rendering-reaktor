#version 150

uniform sampler2D ascii_strip;

flat in float typ;
in vec2 point_cord;

out vec4 frag_color;

void main() {
    if (typ == 1.0) {
        frag_color = mix(vec4(0.0), vec4(1.0),
        	             (1.0 - length(point_cord))
                         * 2.0);
     	// frag_color = vec4(1.0, 0.0, 0.0, 0.0);
    } else if (typ == 2.0) {
        frag_color = texture(ascii_strip, point_cord);
        // frag_color = vec4(1.0, 0.0, 0.0, 1.0);
    }
};
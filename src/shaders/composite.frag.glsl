#version 150 core

in vec3 v_sky_ray;

uniform samplerCube skybox;

out vec4 frag_color;

void main() {
    vec3 color = texture(skybox, v_sky_ray).rgb;

    frag_color = vec4(color, 1.0);
    /* return; */
}

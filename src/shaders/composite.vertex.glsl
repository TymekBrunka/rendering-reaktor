#version 150 core

in vec2 pos;
in float aspectRatio;
in mat4 view;

out vec3 v_sky_ray;

vec3 GetSkyRay(vec2 uv, float fieldOfView, float aspectRatio)
{
    float d = 0.5 / tan(fieldOfView / 2.0);
    return vec3((uv.x - 0.5) * aspectRatio, uv.y - 0.5, -d);
}

void main() {
    vec2 uv = pos * 2.0;
    v_sky_ray = mat3(inverse(view)) * 
            GetSkyRay(uv, aspectRatio, 0.5);
}

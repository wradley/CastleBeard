#version 410 core
in vec3 Distances;

uniform vec3 uColor;
out vec4 FragColor;

void main()
{
    float dst = min(Distances.x, min(Distances.y, Distances.z));
    if (dst > 0.004) discard;
    FragColor = vec4(uColor, 1.0);
}

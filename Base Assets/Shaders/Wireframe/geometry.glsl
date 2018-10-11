#version 410 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 Distances;

float DstFromLine(vec3 A, vec3 B, vec3 P)
{
    vec3 AB = B - A;
    vec3 AP = P - A;
    vec3 ABp = dot(AB, AP) * AB;
    vec3 Bp = ABp + A;
    return length(P - Bp);
}

void main()
{


    vec3 A = gl_in[0].gl_Position.xyz;
    vec3 B = gl_in[1].gl_Position.xyz;
    vec3 C = gl_in[2].gl_Position.xyz;

    vec3 Center = (A + B + C) / 3.0;

    float totalEdgeSize = length(B-A) + length(C-B) + length(A-C);

    Distances = vec3(DstFromLine(A, B, Center), 0.0, 0.0) / totalEdgeSize;
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    Distances = vec3(0.0, DstFromLine(B, C, Center), 0.0) / totalEdgeSize;
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    Distances = vec3(0.0, 0.0, DstFromLine(C, A, Center)) / totalEdgeSize;
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();
}

#version 410 core
in vec3 vNormal;
in vec3 vFragPos;
uniform vec3 uColor;

out vec4 FragColor;

const vec3 lightDir = vec3(-1.0, -1.0, -1.0);
const vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
const vec4 ambientColor = vec4(1.0, 1.0, 1.0, 0.1);

void main() {


    // vec3 color = normalize((fNormal + 1.0) / 2.0);
    // FragColor = vec4(color.x, color.y, color.z, 1.0);

    vec3 ambient = ambientColor.xyz * ambientColor.w;

    vec3 normal = normalize(vNormal);
    float diffuseStrength = max(dot(normal, -lightDir), 0.0);
    vec3 diffuse = diffuseStrength * lightColor.xyz * lightColor.w;

    vec3 color = (ambient + diffuse) * uColor;
    FragColor = vec4(color, 1.0);
}

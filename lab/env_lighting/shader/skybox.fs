#version 330 core
out vec4 fragColor;
in vec3 texCoord;//3d좌표의 방향벡터

uniform samplerCube skybox;//샘플러 큐브?-큐브맵을 만들기위함

void main() {
    fragColor = texture(skybox, texCoord);
}
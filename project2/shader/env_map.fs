#version 330 core

out vec4 fragColor;

in vec3 normal;//vs에서 받는다
in vec3 position;//마찬가지

uniform vec3 cameraPos; 
uniform samplerCube skybox; //skybox와 마찬가지로

void main() {
    /리플렉션벡터    벡터방향으로 부터 큐브맵 텍스쳐 픽셀값을 가져옴
    fragColor = vec4(texture(skybox, R).rgb, 1.0);
}
#version 330 core

layout (location = 0) in vec3 aPos;//큐브형태의 위치만 받는다
out vec3 texCoord;

uniform mat4 transform;

void main() {
  texCoord = aPos;//동일한 텍스쳐 코디네이트
  gl_Position = transform * vec4(aPos, 1.0);
}
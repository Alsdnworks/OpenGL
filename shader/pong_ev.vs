#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
  
out vec3 normal;
out vec2 texCoord;
out vec3 position;
out vec3 envnormal;
out vec3 envposition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 modelTransform;

 
void main() {
    envnormal = mat3(transpose(inverse(model))) * aNormal;
    envposition = vec3(model * vec4(aPos, 1.0));


    texCoord = aTexCoord;

      normal = (transpose(inverse(modelTransform)) * vec4(aNormal, 0.0)).xyz;
      position = (modelTransform * vec4(aPos, 1.0)).xyz;


    gl_Position = projection * view * vec4(position, 1.0);
}

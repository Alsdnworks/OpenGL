#version 330 core
in vec4 vertexColor;
in vec2 texCoord;
out vec4 fragColor;

uniform sampler2D tex;
uniform sampler2D tex2;
//두개의 텍스쳐 6-3 48분즈음
void main(){
    fragColor = (texture(tex,texCoord)*0.8)+(texture(tex2,texCoord)*0.4);
}
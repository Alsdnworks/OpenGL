//위치를 지정한다! 버텍스의 브이!
//3.3코어프로파일 이상에서 작동함을 컴파일러에게 일러준다.
#version 330 core
layout (location = 0) in vec3 aPos;
//셰이더코드도main함수이용
void main() {
    //이하 모든 버텍스에 적용된다
    //(x,y)(x,y)0,0이 중앙기준이다
    gl_Position = vec4(0.5, 0.5, 0.0, 1.0);
}
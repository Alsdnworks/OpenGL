#ifndef __SHADER_H__
#define __SHADER_H__

#include "common.h"
//클래스의 주소를 나타내는 포인터
CLASS_PTR(Shader);
class Shader {
public:
 //셰이더를만들기위해shaderUPtr CreateFromFile만을 사용하게했다.
 //중복오류를 막기위해
 //정적변수 클래스를 쓰는 모든게 공유하는
  static ShaderUPtr CreateFromFile(const std::string& filename, GLenum shaderType);

  ~Shader();
//보통 클래스 셋,겟을 같이쓴다 하지만 은닉성을위해 오픈지엘에서 가져오는 수는 외부편집이 안되고
//오픈지엘만 쓸수있게하는거다
  uint32_t Get() const { return m_shader; }    
private:
  //생성자는 private, 초기화함수도 private,오픈지엘의 셰이더id도 프라이빗
  //
  Shader() {}
  bool LoadFile(const std::string& filename, GLenum shaderType);
  uint32_t m_shader { 0 };
};

#endif // __SHADER_H__
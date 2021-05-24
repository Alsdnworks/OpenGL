#ifndef __SHADER_H__
#define __SHADER_H__

#include "common.h"
//클래스의 주소를 나타내는 포인터
CLASS_PTR(Shader);
class Shader {
public:

  static ShaderUPtr CreateFromFile(const std::string& filename, GLenum shaderType);

  ~Shader();

  uint32_t Get() const { return m_shader; }    
private:
  //
  Shader() {}
  bool LoadFile(const std::string& filename, GLenum shaderType);
  uint32_t m_shader { 0 };
};

#endif // __SHADER_H__
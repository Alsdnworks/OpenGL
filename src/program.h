//vs,fs를 연결하는 파이프라인 프로그램, 최종적으로 그림그리기에 필요한것
//구성단계는 셰이더h와 같으니 이해안가면 거길 참고
#ifndef __PROGRAM_H__
#define __PROGRAM_H__
#include "common.h"
#include "shader.h"

CLASS_PTR(Program)
class Program {
public:
    static ProgramUPtr Create(
        //다른 program을만들때 사용하기위해 유니크가아닌 셰어드포인트ShaderPtr사용
    const std::vector<ShaderPtr>& shaders);
    ~Program();
    uint32_t Get() const { return m_program;} 
    void Use() const;
    // ... in Program class declaration
    void SetUniform(const std::string& name, int value) const;
    void SetUniform(const std::string& name, const glm::mat4& value) const;
private:
    Program() {}
    bool Link(
    const std::vector<ShaderPtr>& shaders);
    uint32_t m_program { 0 };
};

#endif // __PROGRAM_H__

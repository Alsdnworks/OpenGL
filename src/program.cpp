#include "program.h"

ProgramUPtr Program::Create(const std::vector<ShaderPtr> &shaders)
{
    auto program = ProgramUPtr(new Program());
    if (!program->Link(shaders))
        return nullptr;
    return std::move(program);
}

ProgramUPtr Program::Create(
    const std::string &vertShaderFilename,
    const std::string &fragShaderFilename)
{
    ShaderPtr vs = Shader::CreateFromFile(vertShaderFilename,
                                          GL_VERTEX_SHADER);
    ShaderPtr fs = Shader::CreateFromFile(fragShaderFilename,
                                          GL_FRAGMENT_SHADER);
    if (!vs || !fs)
        return nullptr;
    return std::move(Create({vs, fs}));
}

Program::~Program()
{
    if (m_program)
    {
        // 프로그램 오브젝트 제거
        glDeleteProgram(m_program);
    }
}

bool Program::Link(const std::vector<ShaderPtr> &shaders)
{
    //오픈지엘 프로그램 오브젝트 생성
    m_program = glCreateProgram();
    //for(auto i=shaders.begin(); i != shaders.end(), ++i){(*itr)->Get();}과 같다
    //shaders(아마 배열)에있는
    for (auto &shader : shaders)
        //프로그램에 세이더를붙인다. shaders의 내용을 세이더에 순서대로 넣어준다
        glAttachShader(m_program, shader->Get()); //여기까지가 포문의 내용
                                                  //프로그램링크
    glLinkProgram(m_program);

    int success = 0;
    //glGetProgramiv: glad기능으로 program에있는 정수형 정보를 얻어온다
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        //프로그램의 로그를 ㅏ져온다. 링크에러위치 파악에 사용된다
        glGetProgramInfoLog(m_program, 1024, nullptr, infoLog);
        SPDLOG_ERROR("Failed To Link Program:{}", infoLog);
        return false;
    }
    return true;
}

void Program::Use() const
{
    glUseProgram(m_program);
}
void Program::SetUniform(const std::string &name, int value) const
{
    auto loc = glGetUniformLocation(m_program, name.c_str());
    glUniform1i(loc, value);
}

void Program::SetUniform(const std::string &name, const glm::mat4 &value) const
{
    auto loc = glGetUniformLocation(m_program, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Program::SetUniform(const std::string &name, float value) const
{
    auto loc = glGetUniformLocation(m_program, name.c_str());
    glUniform1f(loc, value);
}

void Program::SetUniform(const std::string &name, const glm::vec3 &value) const
{
    auto loc = glGetUniformLocation(m_program, name.c_str());
    //value xyz-> glm::value_ptr(value)
    glUniform3fv(loc, 1, glm::value_ptr(value));
}

void Program::SetUniform(const std::string &name, const glm::vec4 &value) const
{
    auto loc = glGetUniformLocation(m_program, name.c_str());
    glUniform4fv(loc, 1, glm::value_ptr(value));
}
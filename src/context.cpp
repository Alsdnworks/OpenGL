#include "context.h"
//새 컨텍스트 정의
ContextUPtr Context::Create(){
    auto context = ContextUPtr(new Context());
    if (!context->init())
    return nullptr;
    return std::move(context);
}

bool Context::init() {
  //원래 auto로 명시된 유니킆인터를 세어드로 바꿔주었다
    ShaderPtr vertShader = Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/simple.fs", GL_FRAGMENT_SHADER);
   //위에서 널포인터가 하나라도 들어오면 if구문작동 보호코드
    if(!vertShader || !fragShader)
        return false;
    SPDLOG_INFO("vertex shader id: {}", vertShader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragShader->Get());
//벡터로 인자를 넣는데 원래 유니크 포인터이기에 적용이 안되어 위를 쉐어드포인터로 바꿔준다
    m_program = Program::Create({fragShader, vertShader});
    if(!m_program)
        return false;
    SPDLOG_INFO("Program id:{}",m_program->Get());
    //glClearColor()컬러프레임버퍼화면 클리어할 색상지정, 즉 초기화색
    glClearColor(0.2f,0.4f,0.7f,0.1f);


    uint32_t vao=0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);


    return true;
    }
        //glClear()프레임버퍼클리어
        //GL_COLOR_BUFFER_BIT 화면에 보이는 색상버퍼
        //main에서쓰인던 함수를 별개의 호출로 변경
    void Context::Render(){
        //그림이 그려지는부분
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(m_program->Get());
        glDrawArrays(GL_POINTS, 0, 1);
    }
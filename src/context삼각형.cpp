#include "context.h"
//새 컨텍스트 정의
ContextUPtr Context::Create(){
    auto context = ContextUPtr(new Context());
    if (!context->init())
    return nullptr;
    return std::move(context);
}

bool Context::init() {
    //4-5강 3개 버텍스의 위치를 선언
    float vertices[]={
	
    // first triangle
    0.5f, 0.5f, 0.0f, 
    0.5f, -0.5f, 0.0f, 
    -0.5f, 0.5f, 0.0f, 
    };
    //VAO 1개 생성
    glGenVertexArrays(1, &m_vertexArrayObject);
    //지금부터사용할 VAO
    glBindVertexArray(m_vertexArrayObject);
    //자세한설명https://rinthel.github.io/opengl_course/04_graphics_pipeline_and_shader_02#/7
    //버텍스버퍼 생성 및 바인딩
    glGenBuffers(1, &m_vertexBuffer);
             //GL_ARRAY_BUFFER;버텍스버퍼오브젝트지정: 위의 포인터 는 버텍스의 배열 정보가 들어가는 공간이라는뜻
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    //각버텍스는sizeof(float)*9 36바이트공간에 vertices에 복사,GL_STATIC_DRAW
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*12,vertices,GL_STATIC_DRAW);    
//0번어트리뷰트사용
    glEnableVertexAttribArray(0);//simple.vs의 로케이션0
    //4-5강 17:00
//버텍스의 0번째 어트리뷰트, 어트리뷰트는 3개의값으로, float데이터타입, flase노멀라이즈여부, 
//sizeof(float)*3정점간간격,0 첫절점의해당 어트리뷰트까지 간격
    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE,sizeof(float)*3,0);
//참고 VAO바인딩-> VBO바인딩-> 버텍스어트리뷰트세팅수능로 진행 

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

    return true;
    }
        //glClear()프레임버퍼클리어
        //GL_COLOR_BUFFER_BIT 화면에 보이는 색상버퍼
        //main에서쓰인던 함수를 별개의 호출로 변경
    void Context::Render(){
        //그림이 그려지는부분
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(m_program->Get());
        //그리고자하는 타입, 첫정점의 인덱스, 정점의 총 갯수
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
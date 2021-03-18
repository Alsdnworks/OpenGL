#include "context.h"
//새 컨텍스트 정의
ContextUPtr Context::Create(){
    auto context = ContextUPtr(new Context());
    if (!context->init())
    return nullptr;
    return std::move(context);
}

 void Context::Render(){
        //그림이 그려지는부분
        glClear(GL_COLOR_BUFFER_BIT);

        m_program->Use();
        //그리고자하는 타입,EBO내 인뎃그갯수,인덱스데이터형,ebo첫 데이터부터의 오프셋
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    
bool Context::init() {
    //4-5강 3개 버텍스의 위치를 선언
float vertices[] = {
  0.5f, 0.5f, 0.0f, // top right 0
  0.5f, -0.5f, 0.0f, // bottom right 1
  -0.5f, -0.5f, 0.0f, // bottom left 2
  -0.5f, 0.5f, 0.0f, // top left 3
};
//점의 인덱스만으로 삼각형을 만듦을 표시하기위한 어레이
//인덱스의 정수값을저장할 버퍼를 만들어야한다
uint32_t indices[] = { // note that we start from 0!
  0, 1, 3, // first triangle
  1, 2, 3, // second triangle
};
    m_vertexLayout = VertexLayout::Create();
  //아래 두줄 버텍스레이아웃으로 리팩토링됨(윗줄참고)
  /*  //VAO 1개 생성
    glGenVertexArrays(1, &m_vertexArrayObject);
    //지금부터사용할 VAO
    glBindVertexArray(m_vertexArrayObject);*/
    //자세한설명https://rinthel.github.io/opengl_course/04_graphics_pipeline_and_shader_02#/7
    //버텍스버퍼 생성 및 바인딩 -아래주석코드는 리팩토링전 사용
    m_vertexBuffer=Buffer::CreateWithData(GL_ARRAY_BUFFER,GL_STATIC_DRAW,vertices,sizeof(float)*12);
   /* glGenBuffers(1, &m_vertexBuffer);
             //GL_ARRAY_BUFFER;버텍스버퍼오브젝트지정: 위의 포인터 는 버텍스의 배열 정보가 들어가는 공간이라는뜻
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    //각버텍스는sizeof(float)*9 36바이트공간에 vertices에 복사,GL_STATIC_DRAW
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertices, GL_STATIC_DRAW);
    */
//0번어트리뷰트사용
    glEnableVertexAttribArray(0);//simple.vs의 로케이션0
    //4-5강 17:00
//버텍스의 0번째 어트리뷰트, 어트리뷰트는 3개의값으로, float데이터타입, flase노멀라이즈여부, 
//sizeof(float)*3정점간간격,0 첫절점의해당 어트리뷰트까지 간격
    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
//참고 VAO바인딩-> VBO바인딩-> 버텍스어트리뷰트세팅순으로 진행 
	//이밑에 VBO/EBO
    //정수를 쓸거라 어트리뷰트어레이를 쓰지않는다.

    //아래 4줄 buffer.h로 리팩토링됨
    m_indexBuffer=Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER,GL_STATIC_DRAW,indices,sizeof(uint32_t)*6);
   /* glGenBuffers(1, &m_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * 6,
         indices, GL_STATIC_DRAW);*/
  //원래 auto로 명시된 유니킆인터를 세어드로 바꿔주었다 3/16
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
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

    return true;
    }
        //glClear()프레임버퍼클리어
        //GL_COLOR_BUFFER_BIT 화면에 보이는 색상버퍼
        //main에서쓰인던 함수를 별개의 호출로 변경
   
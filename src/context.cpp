#include "context.h"
#include <iostream>
#include "image.h"

ContextUPtr Context::Create() {
    auto context = ContextUPtr(new Context());
    if (!context->Init())
        return nullptr;
    return std::move(context);
}




void Context::Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    m_program->Use();
    glDrawElements(GL_TRIANGLE_STRIP, m_IndexCount, GL_UNSIGNED_INT, 0);
}



bool Context::Init() {
    //텍스처를 추가하기위해 xyzrgb에 st를 추가하였다
    float vertices[] = {
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right, red
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right, green
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ,0.0f, // bottom left, blue
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top left, yellow
    };
    uint32_t indices[] = { // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };
    m_vertexLayout = VertexLayout::Create();
    m_vertexBuffer = Buffer::CreateWithData(
        GL_ARRAY_BUFFER, GL_STATIC_DRAW,
        vertices, sizeof(float) * 32);

    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
    m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 3);
    //텍스쳐 코디네이트 2차원데이터
    m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 6);
    m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 6);

    ShaderPtr vertShader = Shader::CreateFromFile("./shader/texture.vs", GL_VERTEX_SHADER);
    ShaderPtr fragShader = Shader::CreateFromFile("./shader/texture.fs", GL_FRAGMENT_SHADER);
    if (!vertShader || !fragShader)
        return false;
    SPDLOG_INFO("vertex shader id: {}", vertShader->Get());
    SPDLOG_INFO("fragment shader id: {}", fragShader->Get());
    m_program = Program::Create({fragShader, vertShader});
    if (!m_program)
        return false;
    SPDLOG_INFO("program id: {}", m_program->Get());
    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);
//이미지로딩부
     auto image = Image::Load("./image/container.jpg");
    if (!image) 
        return false;
    SPDLOG_INFO("image: {}x{}, {} channels",
            image->GetWidth(), image->GetHeight(), image->GetChannelCount());
//30,46분참조 6-2
     //텍스쳐오브제생성
    glGenTextures(1, &m_texture);
    //사용하고자하는 텍스쳐바인딩
    glBindTexture(GL_TEXTURE_2D, m_texture);
    //텍스쳐필터와 래핑방식등 파라미터를 설정
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//텍스쳐타겟(형식), 베이스레벨,  텍스쳐의 픽셀포맷, w,  h,  텍스쳐외곽크기, 입력하는이미지의픽셀포맷,   입력하는 이미지의 채널별데이터타입, 이미지데이터가기록된 메모리의주소
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->GetWidth(), image->GetHeight(),0, GL_RGB, GL_UNSIGNED_BYTE, image->GetData());

    return true;
}



void Context::CreateCircle(float radius,float s_radius, int segment, int a_userang,int b_userang, float R, float G, float B){
    std::vector<float> b_vertices;
    std::vector<float> a_vertices;
    std::vector<float> vertices;
    std::vector<uint32_t> indices; 
    int ADJ_segment=360/segment;
    segment=segment*ADJ_segment;
    float ADJ_srad=radius-s_radius;
    s_radius=ADJ_srad;
    const float pi =3.141592f;

    for(int i=0; i<segment; i++){
       float angle = 4*pi*i/segment;
       float x =cosf(angle)*radius;
       float y =sinf(angle)*radius;
       float m_x =cosf(angle)*(radius-s_radius);
       float m_y =sinf(angle)*(radius-s_radius);

       
       vertices.push_back(m_x);
       vertices.push_back(m_y);
       vertices.push_back(0);
       vertices.push_back(x);
       vertices.push_back(y);
       vertices.push_back(0);
    }
        for(int i = a_userang; i < b_userang; i++){
            indices.push_back(a_userang+(i-a_userang)%segment);
            indices.push_back(a_userang+(i-a_userang)%segment+1);
            indices.push_back(a_userang+(i-a_userang)%segment+2);
        }
        indices.push_back(b_userang);


    m_vertexLayout = VertexLayout::Create();
    m_vertexBuffer = Buffer::CreateWithData(
        GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices.data(), sizeof(float) * vertices.size()); //.data()벡터함수 내부에서 제공하는 콜백포인터

    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);


    m_indexBuffer = Buffer::CreateWithData(
        GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices.data(), sizeof(uint32_t) * indices.size()); 
        m_IndexCount = (int)indices.size();
        
        for (int i = 0; i < indices.size(); ++i) {
    std::cout << indices[i] << std::endl;
}
      for (int i = 0; i < vertices.size(); ++i) {
    std::cout << vertices[i] << std::endl;
}

    auto loc = glGetUniformLocation(m_program->Get(),"color");
    m_program->Use();
    glUniform4f(loc,R,G,B,1.0f);

}

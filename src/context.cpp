
#include "context.h"
#include <iostream>
#include "image.h"
#include "imgui.h"

ContextUPtr Context::Create(){
  auto context = ContextUPtr(new Context());
  if (!context->CreateCube())
    return nullptr;
  return std::move(context);
}

void Context::ProcessInput(GLFWwindow *window){
  if (!m_cameraControl)
    return;
  const float cameraSpeed = 0.05f;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    m_cameraPos += cameraSpeed * m_cameraFront;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    m_cameraPos -= cameraSpeed * m_cameraFront;

  auto cameraRight = glm::normalize(glm::cross(m_cameraUp, -m_cameraFront));
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    m_cameraPos += cameraSpeed * cameraRight;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    m_cameraPos -= cameraSpeed * cameraRight;

  auto cameraUp = glm::normalize(glm::cross(-m_cameraFront, cameraRight));
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    m_cameraPos += cameraSpeed * cameraUp;
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    m_cameraPos -= cameraSpeed * cameraUp;
}

void Context::Reshape(int width, int height){
  m_width = width;
  m_height = height;
  glViewport(0, 0, m_width, m_height);
}

void Context::MouseMove(double x, double y){
  if (!m_cameraControl)
    return;
  auto pos = glm::vec2((float)x, (float)y);
  auto deltaPos = pos - m_prevMousePos;
  const float cameraRotSpeed = 0.8f;
  m_cameraYaw -= deltaPos.x * cameraRotSpeed;
  m_cameraPitch -= deltaPos.y * cameraRotSpeed;

  if (m_cameraYaw < 0.0f)
    m_cameraYaw += 360.0f;
  if (m_cameraYaw > 360.0f)
    m_cameraYaw -= 360.0f;

  if (m_cameraPitch > 89.0f)
    m_cameraPitch = 89.0f;
  if (m_cameraPitch < -89.0f)
    m_cameraPitch = -89.0f;

  m_prevMousePos = pos;
}

void Context::MouseButton(int button, int action, double x, double y){
  if (button == GLFW_MOUSE_BUTTON_RIGHT){
    if (action == GLFW_PRESS){

      m_prevMousePos = glm::vec2((float)x, (float)y);
      m_cameraControl = true;
    }
    else if (action == GLFW_RELEASE){
      m_cameraControl = false;
    }
  }
}

void Context::CurrentSet(int setcode){ 
if (setcode==0){CurrentSet(seted_primitive);}
if (setcode==1){CreateCube();
seted_primitive=setcode;}
if (setcode==2){CreateCylinder();
seted_primitive=setcode;}
if (setcode==3){CreateSphere();
seted_primitive=setcode;}
if (setcode==4){CreateTorus();
seted_primitive=setcode;}

}

void Context::Render(){
  if (ImGui::Begin("ui window")){
    if (ImGui::ColorEdit4("clear color", glm::value_ptr(m_clearColor))){
      glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
    }
    ImGui::Text("%d   #vertices", (m_VertexCount));
    ImGui::Text("%d   #triangles", (m_IndexCount));
    ImGui::Separator();
    ImGui::DragFloat3("camera pos", glm::value_ptr(m_cameraPos), 0.01f);
    ImGui::DragFloat("camera yaw", &m_cameraYaw, 0.5f);
    ImGui::DragFloat("camera pitch", &m_cameraPitch, 0.5f, -89.0f, 89.0f);
    ImGui::Separator();
    if (ImGui::Button("reset camera")){
      m_cameraYaw = 0.0f;
      m_cameraPitch = 0.0f;
      m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    }
    ImGui::Separator();

    static const char *primitive[]{"Cube", "Cylinder", "Sphere","Torus"};
    static int pre_selected = 0;
    static int pre_currentset = 0;
    ImGui::Combo("primitive", &pre_selected, primitive, IM_ARRAYSIZE(primitive));
    if (pre_selected != pre_currentset){
      switch (pre_selected){
      case 0:
        CreateCube();
        pre_currentset = 0;
        CurrentSet(1);
        break;
      case 1:
        CreateCylinder();
        pre_currentset = 1;
        CurrentSet(2);
        break;
      case 2:
        CreateSphere();
        pre_currentset = 2;
        CurrentSet(3);
        break;
      case 3:
        CreateTorus();
        pre_currentset = 3;
        CurrentSet(4);
        break;        
      default:
        break;
      }
    }

    static const char *textures[]{"wood", "earth", "metal"};
    static int tex_selected = 0;
    static int tex_currentset = 0;
    ImGui::Combo("Texture", &tex_selected, textures, IM_ARRAYSIZE(textures));
    if (tex_selected != tex_currentset){
      switch (tex_selected){
      case 0:
        textureset = "./image/wood.jpg";
        tex_currentset = 0;
        break;
      case 1:
        textureset = "./image/earth.jpg";
        tex_currentset = 1;
        break;
      case 2:
        textureset = "./image/metal.jpg";
        tex_currentset = 2;
        break;
      default:
        break;
      }
      CurrentSet(0);
      tex_currentset = tex_selected;
    }
    ImGui::Separator();
    ImGui::DragFloat3("rotation", glm::value_ptr(matrot), 0.1f);
    ImGui::DragFloat3("scale", glm::value_ptr(matsca), 0.02f);
    for (int i = 0; i < 3; i++){
      if (matrot[i] < 0.0f)
        matrot[i] += 360.0f;
      if (matrot[i] > 360.0f)
        matrot[i] -= 360.0f;
    }

    static bool isanimation;
    ImGui::Checkbox("animation", &isanimation);
    ImGui::DragFloat3("rot speed", glm::value_ptr(matspd), 0.1f);
    for (int i = 0; i < 3; i++){
      matspd[i] = matspd[i] * isanimation;
    }
    if (ImGui::Button("reset transform")){
      matspd = glm::vec3(0.0f, 0.0f, 0.0f);
      matrot = glm::vec3(0.0f, 0.0f, 0.0f);
      matsca = glm::vec3(1.0f, 1.0f, 1.0f);
      isanimation = false;
    }
  }
  ImGui::End();
  std::vector<glm::vec3> Position = {glm::vec3(0.0f, 0.0f, 0.0f)};

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  m_cameraFront =
      glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraYaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
      glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraPitch), glm::vec3(1.0f, 0.0f, 0.0f)) *
      glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

  auto projection = glm::perspective(glm::radians(45.0f),
                                     (float)m_width / (float)m_height, 0.01f, 20.0f);
  auto view = glm::lookAt(
      m_cameraPos,
      m_cameraPos + m_cameraFront,
      m_cameraUp);

  glm::mat4 scaleMatrix = glm::mat4(1.0f);
  scaleMatrix[0][0] = matsca[0];
  scaleMatrix[1][1] = matsca[1];
  scaleMatrix[2][2] = matsca[2];
  glm::mat4 rotXMatrix = glm::mat4(1.0f);
  rotXMatrix[1][1] = cos((matspd[0] * (float)glfwGetTime()) + matrot[0] / 57.3f);
  rotXMatrix[1][2] = -sin((matspd[0] * (float)glfwGetTime()) + matrot[0] / 57.3f);
  rotXMatrix[2][1] = sin((matspd[0] * (float)glfwGetTime()) + matrot[0] / 57.3f);
  rotXMatrix[2][2] = cos((matspd[0] * (float)glfwGetTime()) + matrot[0] / 57.3f);
  glm::mat4 rotYMatrix = glm::mat4(1.0f);
  rotYMatrix[0][0] = cos((matspd[1] * (float)glfwGetTime()) + matrot[1] / 57.3f);
  rotYMatrix[0][2] = sin((matspd[1] * (float)glfwGetTime()) + matrot[1] / 57.3f);
  rotYMatrix[2][0] = -sin((matspd[1] * (float)glfwGetTime()) + matrot[1] / 57.3f);
  rotYMatrix[2][2] = cos((matspd[1] * (float)glfwGetTime()) + matrot[1] / 57.3f);
  glm::mat4 rotZMatrix = glm::mat4(1.0f);
  rotZMatrix[0][0] = cos((matspd[2] * (float)glfwGetTime()) + matrot[2] / 57.3f);
  rotZMatrix[0][1] = -sin((matspd[2] * (float)glfwGetTime()) + matrot[2] / 57.3f);
  rotZMatrix[1][0] = sin((matspd[2] * (float)glfwGetTime()) + matrot[2] / 57.3f);
  rotZMatrix[1][1] = cos((matspd[2] * (float)glfwGetTime()) + matrot[2] / 57.3f);

  glm::mat4 materialSet = rotZMatrix * rotYMatrix * rotXMatrix * scaleMatrix;
  auto transform = projection * view * materialSet;
  m_program->SetUniform("transform", transform);
//-------------------------------------------------------------------------------------------디버그시 GL_LINE_STRIP
  glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0);
}

bool Context::CreateCube() {   
    float vertices[] = {        
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
       0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
       0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,

      -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
       0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
       0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

      -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

       0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
       0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
       0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
       0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
       0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
       0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
      -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

      -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
       0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
       0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
    };
    uint32_t indices[] = {
       0,  2,  1,  2,  0,  3,
       4,  5,  6,  6,  7,  4,
       8,  9, 10, 10, 11,  8,
      12, 14, 13, 14, 12, 15,
      16, 17, 18, 18, 19, 16,
      20, 22, 21, 22, 20, 23,
    };
    m_VertexCount = sizeof(vertices) / 4;
    m_IndexCount = sizeof(indices) / 3;
    m_vertexLayout = VertexLayout::Create();
    m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * (5 * 24));
    m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
    m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, sizeof(float) * 3);
    m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * (6 * 6));
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
    auto image = Image::Load(textureset);
    if (!image)
      return false;
    SPDLOG_INFO("image: {}x{}, {} channels",
                image->GetWidth(), image->GetHeight(), image->GetChannelCount());
    m_texture = Texture::CreateFromImage(image.get());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture->Get());
    m_program->Use();
    m_program->SetUniform("tex", 0);

    return true;
}
/*함수식으로 구현하기위해선 vx는 y=0.00003(x-180)^2+1
vy= 기본형 ax^3+bx^2+cx+d로 구해야한다.*/

bool Context::CreateCylinder(){
  std::vector<float> vertices;
  std::vector<uint32_t> indices;
  const float pi = 3.141592f;
  for (int i = 0; i < 360; i++){
    float angle = 4 * pi * i / 360;
    float x = cosf(angle);
    float y = sinf(angle);
    float vx;
    float vy;
    vertices.push_back(0);
    vertices.push_back(0);
    vertices.push_back(0.5f);
    vertices.push_back(0.5);
    vertices.push_back(0.5);

    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(0.5f);
    if(i<180){
      vx=1/180;
    }
    if(i>180){
      vx=1-(1/180);
    }    
    vertices.push_back(vx);
    if(i<=90){
      vy=0.5-((0.5/90)*i);
    }
    if(i>=90){
      vy=((0.5/90)*i);
    }
    if(i>=180){
      vy=0.5+((0.5/90)*i);
    }
    if(i>=270){
      vy=1-((0.5/90)*i);
    }
    vertices.push_back(vy);

    vertices.push_back(0);
    vertices.push_back(0);
    vertices.push_back(-0.5f);
    vertices.push_back(0.5);
    vertices.push_back(0.5);

    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(-0.5f);
    vertices.push_back(vx);
    vertices.push_back(vy);
  }
   //중점은 0.0 xy툭이있는 버텍스에서는 텍스쳐설정을 어떻게?
   //시작점은 0.0 0.5 쿼터일때 0.5 0.0 반일때 1.0 0.5 다음쿼터일때 0.5 1.0 
  for (int i = 0; i < 720; i++){
    if (i % 4 == 0){
      indices.push_back(i % 720 + 2);
      indices.push_back(i % 720 + 3);
      indices.push_back(i % 720 + 7);
    }
  }

  for (int i = 0; i < 720; i++)
  {
    if (i % 4 == 0){
      indices.push_back(i % 720);
      indices.push_back(i % 720 + 1);
      indices.push_back(i % 720 + 5);
    }
  }
   
  for (int i = 1; i  < 720; i += 2){
    indices.push_back(i % 720);
    indices.push_back(i % 720 + 2);
    indices.push_back(i % 720 + 4);
  }

  m_vertexLayout = VertexLayout::Create();
  m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices.data(), sizeof(float) * vertices.size());
  m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
  m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, sizeof(float) * 3);
  m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices.data(), sizeof(uint32_t) * indices.size());
  m_VertexCount = (int)indices.size();
  m_IndexCount = (int)vertices.size();
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

  auto image = Image::Load(textureset);
  if (!image)
    return false;
  SPDLOG_INFO("image: {}x{}, {} channels",
              image->GetWidth(), image->GetHeight(), image->GetChannelCount());
  m_texture = Texture::CreateFromImage(image.get());
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_texture->Get());
  m_program->Use();
  m_program->SetUniform("tex", 0);
  return true;
}
//구는 직접구현하지못해 인터넷에서 갸져온 소스 사용
bool Context::CreateSphere(){
  std::vector<float> vertices;
  std::vector<int> indices;
  const float pi = 3.141592f;
   
  for (int y = 0; y <= 360; y++)
  {
    for (int x = 0; x <= 360; x++)
    {
      float xSegment = (float)x / (float)360;
      float ySegment = (float)y / (float)360;
      float xPos = cos(xSegment * 2.0f * pi) * sin(ySegment * pi);
      float yPos = cos(ySegment * pi);
      float zPos = sin(xSegment * 2.0f * pi) * sin(ySegment * pi);
      vertices.push_back(xPos);
      vertices.push_back(yPos);
      vertices.push_back(zPos);   
    }
  }
   
  for (int i = 0; i < 180; i++){
    for (int j = 0; j < 180; j++){
      indices.push_back(i * 360 + j);
      indices.push_back((i + 1) * 360 + j);
      indices.push_back((i + 1) * 360 + j + 1);

      indices.push_back(i * 360 + j);
      indices.push_back((i + 1) *360 + j + 1);
      indices.push_back(i * 360+ j + 1);
    }
  }

  m_vertexLayout = VertexLayout::Create();
  m_vertexBuffer = Buffer::CreateWithData(
      GL_ARRAY_BUFFER, GL_STATIC_DRAW,
      vertices.data(), sizeof(float) * vertices.size());
  m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
  m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_TRUE, sizeof(float) * 3, 0);
  m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices.data(), sizeof(uint32_t) * indices.size());
  m_VertexCount = (int)indices.size();
  m_IndexCount = (int)vertices.size();
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
  auto image = Image::Load(textureset);
  if (!image)
    return false;
  SPDLOG_INFO("image: {}x{}, {} channels",
              image->GetWidth(), image->GetHeight(), image->GetChannelCount());
  m_texture = Texture::CreateFromImage(image.get());
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_texture->Get());
  m_program->Use();
  m_program->SetUniform("tex", 0);
  return true;
}

bool Context::CreateTorus(){
    std::vector<float> vertices;
  std::vector<uint32_t> indices;
  const float pi = 3.141592f;
  for (int i = 0; i < 360; i++){
    float angle = 4 * pi * i / 360;
    float x = cosf(angle);
    float y = sinf(angle);
    vertices.push_back(0.0f + (x / 1.5f));
    vertices.push_back(0.0f + (y / 1.5f));
    vertices.push_back(0.5f);
    vertices.push_back(0.014f * (float)i);
    vertices.push_back(0.014f * (float)i);

    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(0.5f);
    vertices.push_back(0.014f * i);
    vertices.push_back(0.014f * i);

    vertices.push_back(0.0f + (x / 1.5f));
    vertices.push_back(0.0f + (y / 1.5f));
    vertices.push_back(-0.5f);
    vertices.push_back(0.014f * i);
    vertices.push_back(0.014f * i);

    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(-0.5f);
    vertices.push_back(0.014f * i);
    vertices.push_back(0.014f * i);
  }

  for (int i = 0; i < 720; i++){
    if (i % 4 == 0){
      indices.push_back(i % 720 + 2);
      indices.push_back(i % 720 + 3);
      indices.push_back(i % 720 + 6);
      indices.push_back(i % 720 + 3);
      indices.push_back(i % 720 + 6);
      indices.push_back(i % 720 + 7);
    }
  }

  for (int i = 0; i < 720; i++){
    if (i % 4 == 0){
      indices.push_back(i % 720);
      indices.push_back(i % 720 + 1);
      indices.push_back(i % 720 + 4);
      indices.push_back(i % 720 + 1);
      indices.push_back(i % 720 + 4);
      indices.push_back(i % 720 + 5);
    }
  }

  for (int i = 1; i < 720; i += 2){
    indices.push_back(i % 720);
    indices.push_back(i % 720 + 2);
    indices.push_back(i % 720 + 4);
  }

  for (int i = 0; i < 720; i += 2){
    indices.push_back(i % 720);
    indices.push_back(i % 720 + 2);
    indices.push_back(i % 720 + 4);
  }
  m_vertexLayout = VertexLayout::Create();
  m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices.data(), sizeof(float) * vertices.size());
  m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
  m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, sizeof(float) * 3);
  m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices.data(), sizeof(uint32_t) * indices.size());
  m_VertexCount = (int)indices.size();
  m_IndexCount = (int)vertices.size();
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

  auto image = Image::Load(textureset);
  if (!image)
    return false;
  SPDLOG_INFO("image: {}x{}, {} channels",
              image->GetWidth(), image->GetHeight(), image->GetChannelCount());
  m_texture = Texture::CreateFromImage(image.get());
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_texture->Get());
  m_program->Use();
  m_program->SetUniform("tex", 0);
  return true;
}
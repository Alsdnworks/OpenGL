#include "context.h"
#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void OnFramebufferSizeChange(GLFWwindow* window, int width, int height){
  SPDLOG_INFO("framebuffer size changed: ({} x {})", width, height);
  auto context = (Context*)glfwGetWindowUserPointer(window);
  context->Reshape(width,height);
  //7-5 23분에 타입캐스팅참조
  }

//키로그를 찍는코드와 key == GLFW_KEY_ESCAPE(이에스시)가 눌리면 종료를 구현하는함수 
void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    SPDLOG_INFO("key: {}, scancode: {}, action: {}, mods: {}{}{}",
        key, scancode,
        action == GLFW_PRESS ? "Pressed" :
        action == GLFW_RELEASE ? "Released" :
        action == GLFW_REPEAT ? "Repeat" : "Unknown",
        mods & GLFW_MOD_CONTROL ? "C" : "-",
        mods & GLFW_MOD_SHIFT ? "S" : "-",
        mods & GLFW_MOD_ALT ? "A" : "-");
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void OnCursorPos(GLFWwindow* window, double x, double y) {
     auto context = (Context*)glfwGetWindowUserPointer(window);
     context->MouseMove(x, y);
}

void OnMouseButton(GLFWwindow* window, int button, int action, int modifier) {
  ImGui_ImplGlfw_MouseButtonCallback(window, button, action, modifier);
  auto context = (Context*)glfwGetWindowUserPointer(window);
  double x, y;
  glfwGetCursorPos(window, &x, &y);
  context->MouseButton(button, action, x, y);
}  

void OnCharEvent(GLFWwindow* window, unsigned int ch){
    ImGui_ImplGlfw_CharCallback(window,ch);
}
void OnScroll(GLFWwindow* window, double xoffset, double yoffset) {
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
}

int main(int argc, const char** argv) {
    // 시작을 알리는 로그
    SPDLOG_INFO("Start program");
    SPDLOG_INFO("arg count:{}",argc);
    for(int i=0; i<argc; i++){
        SPDLOG_INFO("argv[{}]:{}", i, argv[i]);
    }

    // glfw 라이브러리 초기화, 실패하면 에러 출력후 종료 에러코드는 -1
    SPDLOG_INFO("Initialize glfw");
    if (!glfwInit()) {
        const char* description = nullptr;
        glfwGetError(&description);
        SPDLOG_ERROR("failed to initialize glfw: {}", description);
        return -1;
    }

    //window의 opengl contextversion을 명시 에러코드는 -1
    //코어프로파일
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);//12-3 MSAA 활성화요청 왜? 버퍼어레이를 재구성

        // glfw 윈도우 생성, 실패하면 에러 출력후 종료
    SPDLOG_INFO("Create glfw window");
    auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME,
      nullptr, nullptr);
    if (!window) {
        SPDLOG_ERROR("failed to create glfw window");
        return -1;
    }

//앞으로auto window=glfwCreateWindow에서 생성된 opengl context를 사용합니다.
        glfwMakeContextCurrent(window);


    // glad를 활용한 OpenGL 함수 로딩 실패시 에러코드는 -1
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
     SPDLOG_ERROR("failed to initialize glad");
     glfwTerminate();
     return -1;
    }
    //glgetstring 기본 함수를 로드한다
    //opengl context 버전 표시
    auto glVersion = glGetString(GL_VERSION);
    SPDLOG_INFO("OpenGL context version: {}", glVersion);

    auto imguiContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(imguiContext);
    ImGui_ImplGlfw_InitForOpenGL(window, false);
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplOpenGL3_CreateFontsTexture();
    ImGui_ImplOpenGL3_CreateDeviceObjects();

    //컨텍스트를 이용한 초기화
    auto context = Context::Create();
    if(!context){
        SPDLOG_ERROR("failed to create context");
        glfwTerminate();
        return -1;
    }
    context->Init();
    //이벤트 처리단
    glfwSetWindowUserPointer(window,context.get());
    //윈도우가 처음생성되었을때 아래줄코드(사이즈표시)를 실행해주는부분
    OnFramebufferSizeChange(window, WINDOW_WIDTH, WINDOW_HEIGHT);
    //윈도우 크기 변경 이벤트를 수집했을때 OnFramebufferSizeChange함수를 호출   
    glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChange);
    //윈도우 키 입력 이벤트를 수집했을때 OnKeyEvent함수를 호출   
    glfwSetKeyCallback(window, OnKeyEvent);
    glfwSetCursorPosCallback(window,OnCursorPos);
    glfwSetMouseButtonCallback(window, OnMouseButton);
    glfwSetCharCallback(window, OnCharEvent);
    glfwSetScrollCallback(window, OnScroll);
     // glfw 루프 실행, 윈도우 close 버튼을 누르면 정상 종료
    SPDLOG_INFO("Start main loop");

    // while~ 창이 닫힌다가 부정인동안(즉 열려있는동안)
    while (!glfwWindowShouldClose(window)) {
   //이벤트를 수집한다
        glfwPollEvents();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        context->Render();
        context->ProcessInput(window);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        //프레임버퍼는 2개 화면을 확장하면 백버퍼에 그림을 그리고 준비가되면
        //프론트버퍼와 스왑해준다.(밑의 3줄) 이는 부드러운 화면전환이 가능하게해준다.
        glfwSwapBuffers(window);
    }
    //컨텍스트 사용후 초기화, 아님 컨텍스트에 널포인트를 넣어 소유권해제
    //context=nullptr;
    context.reset();
    ImGui_ImplOpenGL3_DestroyFontsTexture();
    ImGui_ImplOpenGL3_DestroyDeviceObjects();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(imguiContext);
    glfwTerminate();
    return 0;
} 
//State-setting function
//State가 OpenGL context에 저장됨
//glViewport(), glClearColor()
//State-using function
//OpenGL context에 저장된 State를 이용
//glClear()

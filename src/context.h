//리팩토리작업에 사용될 파일이다 GLFW함수 OpenGL CONTEXT GLAD를 분리해둔다
#ifndef __CONTEXT_H__
#define __CONTEXT_H__
#include "common.h"
#include "shader.h"
#include "program.h"
#include "buffer.h"
#include "vertex_layout.h"
CLASS_PTR(Context)
class Context{
    public:
        static ContextUPtr Create();
        void Render();
    private:
        Context(){}
        bool init();
        ProgramUPtr m_program;
//VAO/VBO를 담아둘 버퍼지정 https://velog.io/@huijae0817/OpenGLVAO-%EC%99%80-VBO%EC%9D%91%EC%9A%A9
//vao=버텍스데이터구조를 알려주는 오브젝트,각 정점은 몇바이트로구성/ 분리되었는지,
//n번째데이터는 어떤사이즈의 데이터가 몇개? 이런걸 담는다
        VertexLayoutUPtr m_vertexLayout;
        BufferUPtr m_vertexBuffer;
        BufferUPtr m_indexBuffer;
};
#endif // __CONTEXT_H__
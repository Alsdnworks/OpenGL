//리팩토리작업에 사용될 파일이다 GLFW함수/ OpenGL CONTEXT/ GLAD를 분리해둔다
#ifndef __CONTEXT_H__
#define __CONTEXT_H__
#include "common.h"
#include "shader.h"
#include "program.h"

CLASS_PTR(Context)
class Context{
    public:
        static ContextUPtr Create();
        void Render();
    private:
        Context(){}
        bool init();
        ProgramUPtr m_program;
};
#endif




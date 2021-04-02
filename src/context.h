#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "program.h"
#include "buffer.h"
#include "vertex_layout.h"

CLASS_PTR(Context)
class Context {
public:
    static ContextUPtr Create();
    void Render();    
                                     bool Init();

    void CreateCircle(float radius,float s_radius, int segment,int a_userang,int b_userang, float R, float G, float B);
private:
    Context() {}
    
    ProgramUPtr m_program;

    VertexLayoutUPtr m_vertexLayout;
    BufferUPtr m_vertexBuffer;
    BufferUPtr m_indexBuffer;
    int m_IndexCount{6};
    uint32_t m_texture;
};

#endif // __CONTEXT_H__
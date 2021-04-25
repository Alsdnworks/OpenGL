#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "program.h"
#include "buffer.h"
#include "vertex_layout.h"
#include "texture.h"

CLASS_PTR(Context)
class Context {
public:
    static ContextUPtr Create();
    void Render();
    void ProcessInput(GLFWwindow* window);
     bool Init();
     void Reshape(int width, int height);
     void MouseMove(double x, double y);
     void MouseButton(int button, int action, double x, double y);
     void CreateCircle(float radius,float s_radius, int segment,int a_userang,int b_userang, float R, float G, float B);

private:
    Context() {}
    const char* textureset="./image/wood.jpg";
    ProgramUPtr m_program;
    VertexLayoutUPtr m_vertexLayout;
    BufferUPtr m_vertexBuffer;
    BufferUPtr m_indexBuffer;
    int m_IndexCount{6};
    TextureUPtr m_texture;
    //clear color
    glm::vec4 m_clearColor {glm::vec4(0.0f,0.0f,0.0f,0.0f)};	
    // camera parameter
    bool m_cameraControl { false };
    glm::vec2 m_prevMousePos { glm::vec2(0.0f) };    
    float m_cameraPitch{0.0f};
    float m_cameraYaw{0.0f};
    glm::vec3 m_cameraPos { glm::vec3(0.0f, 0.0f, 3.0f) };
    glm::vec3 m_cameraFront { glm::vec3(0.0f, 0.0f, -1.0f) };
    glm::vec3 m_cameraUp { glm::vec3(0.0f, 1.0f, 0.0f) };
    glm::vec3 matrot{glm::vec3(0.0f,0.0f,0.0f)};
    glm::vec3 matsca{glm::vec3(1.0f,1.0f,.0f)};

    int m_width{WINDOW_WIDTH};
    int m_height{WINDOW_HEIGHT};

};

#endif // __CONTEXT_H__
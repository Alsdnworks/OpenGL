#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "program.h"
#include "buffer.h"
#include "vertex_layout.h"
#include "texture.h"
#include "mesh.h"
#include "model.h"
#include "framebuffer.h"

CLASS_PTR(Context)
class Context{
public:
  static ContextUPtr Create();
  void Render();
  void ProcessInput(GLFWwindow *window);
  bool Init();
  void Reshape(int width, int height);
  void MouseMove(double x, double y);
  void MouseButton(int button, int action, double x, double y);

private:
  Context() {}
  ProgramUPtr m_program;
  ProgramUPtr m_simpleProgram;
  ProgramUPtr m_textureProgram;
  ProgramUPtr m_postProgram;
  float m_gamma {1.0f};
  float m_envScale {0.0f};
  float m_param {0.0f};
  MeshUPtr m_box;
  MeshUPtr m_plane;
  TextureUPtr m_texture;
  TextureUPtr m_texture2;

  //ModelUPtr m_model;
  bool m_animation{true};

  // light parameter
  struct Light{
    glm::vec3 position { glm::vec3(1.0f, 4.0f, 4.0f) };
    glm::vec3 direction { glm::vec3(-1.0f, -1.0f, -1.0f) };
    glm::vec2 cutoff { glm::vec2(120.0f, 5.0f) };
    float distance { 128.0f };
    glm::vec3 ambient { glm::vec3(0.1f, 0.1f, 0.1f) };
    glm::vec3 diffuse { glm::vec3(0.8f, 0.8f, 0.8f) };
    glm::vec3 specular { glm::vec3(1.0f, 1.0f, 1.0f) };
  };
  Light m_light;
  bool m_flashlightMode {false};

  MaterialPtr m_planeMaterial;
  MaterialPtr m_box1Material;
  MaterialPtr m_box2Material;
  TexturePtr m_windowTexture;
  MaterialPtr m_helmetMaterial;

  
  //clear color
  glm::vec4 m_clearColor{glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)};
  // camera parameter
  bool m_cameraControl{false};
  glm::vec2 m_prevMousePos{glm::vec2(0.0f)};
  float m_cameraPitch{2.5f};
  float m_cameraYaw{0.0f};
  glm::vec3 m_cameraPos{glm::vec3(0.0f, 1.5f, 7.0f)};
  glm::vec3 m_cameraFront{glm::vec3(0.0f, 0.0f, -1.0f)};
  glm::vec3 m_cameraUp{glm::vec3(0.0f, 1.0f, 0.0f)};
  //Framebuffer
  FramebufferUPtr m_framebuffer;
  // cubemap
  CubeTextureUPtr m_cubeTexture;
  ProgramUPtr m_skyboxProgram;
  ProgramUPtr m_envMapProgram;
  //grass
  TexturePtr m_grassTexture;
  ProgramUPtr m_grassProgram;
  std::vector<glm::vec3>m_grassPos;
  BufferUPtr m_grassPosBuffer;
  VertexLayoutUPtr m_grassInstance;
  //helmet
  ModelUPtr m_model;
  //env+pong
  ProgramUPtr m_combinedProgram;
  float m_shaderOption{0.0f};

  int m_width{WINDOW_WIDTH};
  int m_height{WINDOW_HEIGHT};
};

#endif // __CONTEXT_H__
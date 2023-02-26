#ifndef _PHOENIXGFXENGINE_H
#define _PHOENIXGFXENGINE_H

#include <iostream>
#include <string>
#include <vector>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

struct Shader {
  public:
    std::string _filePath;
    GLenum _shaderType;
};

class PhoenixGfxEngine {
  private:
    int _height;
    int _width;
    GLFWwindow* _window;
    GLuint _program;
    std::vector<Shader*> _shaders;
    GLuint _VAO;
    GLint _time;

    void Init();
    GLuint loadShader(const std::string& filePath, GLenum shaderType);
    
  public:
    PhoenixGfxEngine(int height, int width);
    void LoadNewShader(Shader* pshader);
    void SubmitShaders();
    void Run();

    ~PhoenixGfxEngine();
};

#endif
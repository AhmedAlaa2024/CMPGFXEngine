#include <iostream>
#include <fstream>
#include <string>
#include <PhoenixGfxEngine.h>

int main(int, char**) {

  PhoenixGfxEngine engine(640, 320);
  Shader shader = {"../assets/shaders/simple.vert", GL_VERTEX_SHADER};
  engine.LoadNewShader(&shader);
  shader._filePath = "../assets/shaders/simple.frag";
  shader._shaderType = GL_FRAGMENT_SHADER;
  engine.LoadNewShader(&shader);

  engine.SubmitShaders();

  engine.Run();

  return 0;
}
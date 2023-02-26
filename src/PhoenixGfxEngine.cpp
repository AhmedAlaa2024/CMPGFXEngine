#include <PhoenixGfxEngine.h>

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

void PhoenixGfxEngine::Init() {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW!" << std::endl;
    exit(-1); 
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  //Make window size fixed (User can't resize it)
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  //Set Number of sample used in MSAA (0 = Disabled)
  glfwWindowHint(GLFW_SAMPLES, 16);

  //Enable Double Buffering
  glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

  //Set the bit-depths of the frame buffer
  glfwWindowHint(GLFW_RED_BITS, 8);
  glfwWindowHint(GLFW_GREEN_BITS, 8);
  glfwWindowHint(GLFW_BLUE_BITS, 8);
  glfwWindowHint(GLFW_ALPHA_BITS, 8);

  //Set Bits for Depth Buffer
  glfwWindowHint(GLFW_DEPTH_BITS, 24);
  //Set Bits for Stencil Buffer
  glfwWindowHint(GLFW_STENCIL_BITS, 8);

  //Set the refresh rate of the window (GLFW_DONT_CARE = Run as fast as possible)
  glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);

  _window = glfwCreateWindow(
    _height, _width, "Tutorial", nullptr, nullptr
  );

  if (!_window) {
    glfwTerminate();
    std::cerr << "Failed to create window!" <<  std::endl;
    exit(-1);
  }

  glfwMakeContextCurrent(_window);

  gladLoadGL(glfwGetProcAddress);

  _program = glCreateProgram();
}

PhoenixGfxEngine::PhoenixGfxEngine(int height, int width) {
  _height = height;
  _width = width;

  Init();
}

GLuint PhoenixGfxEngine::loadShader(const std::string& filePath, GLenum shaderType) {
  GLuint shader = glCreateShader(shaderType);

  std::ifstream file(filePath);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filePath << std::endl;
    exit(-1);
  }

  std::string source = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

  if (source.empty()) {
    std::cerr << "Failed to load shader source code from " << filePath << std::endl;
    exit(-1);
  }

  const char* sourceCStr = source.c_str();

  glShaderSource(shader, 1, &sourceCStr, nullptr);
  glCompileShader(shader);

  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cerr << "Failed to compile shader: " << infoLog << std::endl;
    exit(-1);
  }

  return shader;
}

void PhoenixGfxEngine::LoadNewShader(Shader* pshader) {
  if (!pshader) {
    throw std::runtime_error("Failed to load the shader : pshader is pointing to null!");
  } else {
    Shader *shader = new Shader;
    shader->_filePath = pshader->_filePath;
    shader->_shaderType = pshader->_shaderType;

    _shaders.push_back(shader);
  }
}

void PhoenixGfxEngine::SubmitShaders() {
  for (struct Shader* pshader : _shaders) {
    GLuint sh = loadShader(pshader->_filePath, pshader->_shaderType);
    glAttachShader(_program, sh);
    glDeleteShader(sh);
  }

  glLinkProgram(_program);
}

void PhoenixGfxEngine::Run() {
  glGenVertexArrays(1, &_VAO);

  /* Carring the address of the time variable */
  _time = glGetUniformLocation(_program, "time");

  while (!glfwWindowShouldClose(_window)) {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(_VAO);
    glUseProgram(_program);

    glUniform1f(_time, (float)glfwGetTime());
    
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(_window);

    glfwPollEvents();
  }
}

PhoenixGfxEngine::~PhoenixGfxEngine() {
  glfwDestroyWindow(_window);
  glfwTerminate();
}
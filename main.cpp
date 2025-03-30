// #include <GL/glew.h>
// #include <GLFW/glfw3.h>
#include <glew.h>
#include <glfw3.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <windows.h>

#include <imgui/backend/imgui_impl_glfw.h>
#include <imgui/backend/imgui_impl_opengl3.h>

#include <Credentials.h>

using namespace std;

constexpr int NUM_VAOS = 1;

struct Uniforms {
  GLuint offset;
  GLuint time;
  GLuint width;
  GLuint height;
  GLuint type;
  GLfloat pos;
  GLuint rotate;
  GLuint render;
  GLfloat mouseX;
  GLfloat mouseY;

  void init(GLuint program) {
    offset = glGetUniformLocation(program, "offset");
    time = glGetUniformLocation(program, "time");
    width = glGetUniformLocation(program, "width");
    height = glGetUniformLocation(program, "height");
    type = glGetUniformLocation(program, "type");
    pos = glGetUniformLocation(program, "posOffset");
    rotate = glGetUniformLocation(program, "rotateSpeed");
    render = glGetUniformLocation(program, "renderSteps");
    mouseX = glGetUniformLocation(program, "mouseX");
    mouseY = glGetUniformLocation(program, "mouseY");
  }
};

struct AppData {
  int width;
  int height;

  const char* vertexPath;
  const char* fragmentPath;

  GLuint renderingProgram;
  GLuint vShader;
  GLuint fShader;
  GLuint vao[NUM_VAOS];

  int   menuCase;
  float defValue;
  int   defRotationSpeed;
  int   defRenderAmount;

  float x;
  float t;
  float inc;
  float tinc;

  bool  isLeftCtrlDown;
  bool  isLeftMouseDown;

  double mouseX, mouseY;
  double dx, dy;
  double offsetX, offsetY;
  double totalX, totalY;

  Uniforms uniforms;
};

static void printOpenGLVersionInfo() 
{
  cout << "Vendor:   " << glGetString(GL_VENDOR) << endl;
  cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
  cout << "Version:  " << glGetString(GL_VERSION) << endl;
  cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

static void windowTerminate(GLFWwindow* window) 
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

static std::string readShaderSource(const std::string& filepath) 
{
  std::ifstream file(filepath, std::ios::ate | std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + filepath);
  }

  size_t fileSize = static_cast<size_t>(file.tellg());
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);
  file.close();

  return std::string(buffer.begin(), buffer.end());
}

static GLuint createShader(const char* glslCode, GLenum type) 
{
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &glslCode, nullptr);
  glCompileShader(shader);
  return shader;
}

static void printProgramLog(GLuint prog) 
{
  int len = 0;
  glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
  if (len > 0) {
    std::vector<char> log(len);
    glGetProgramInfoLog(prog, len, nullptr, log.data());
    std::cout << "Program Info Log: " << log.data() << std::endl;
  }
}

static GLuint createShaderProgram(AppData& data, int resetShader)
{
  if (resetShader == 1) {
    glDeleteProgram(data.renderingProgram);
  }

  std::string vertexShaderStr   = readShaderSource(data.vertexPath);
  std::string fragmentShaderStr = readShaderSource(data.fragmentPath);
  const char* vertexShader      = vertexShaderStr.c_str();
  const char* fshaderSource     = fragmentShaderStr.c_str();

  data.vShader = createShader(vertexShader, GL_VERTEX_SHADER);
  data.fShader = createShader(fshaderSource, GL_FRAGMENT_SHADER);

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, data.vShader);
  glAttachShader(shaderProgram, data.fShader);
  glLinkProgram(shaderProgram);

  glDeleteShader(data.vShader);
  glDeleteShader(data.fShader);

  GLint result;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
  if (result != GL_TRUE) {
    printProgramLog(shaderProgram);
  }

  return shaderProgram;
}

void initializeUniforms(AppData& data) 
{
  data.uniforms.init(data.renderingProgram);
}

static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
  AppData* data = reinterpret_cast<AppData*>(glfwGetWindowUserPointer(window));
  if (!data) return;

  if (key == GLFW_KEY_LEFT_CONTROL) {
    data->isLeftCtrlDown = (action == GLFW_PRESS);
  }

  if (data->isLeftCtrlDown) {
    switch (key) {
    case GLFW_KEY_R:
      data->renderingProgram = createShaderProgram(*data, 1);
      break;
    case GLFW_KEY_1:
      data->menuCase = 0;
      break;
    case GLFW_KEY_2:
      data->menuCase = 1;
      break;
    case GLFW_KEY_3:
      data->menuCase = 2;
      break;
    case GLFW_KEY_4:
      data->menuCase = 3;
      break;
    default:
      break;
    }
  }
}

static void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
  AppData* data = reinterpret_cast<AppData*>(glfwGetWindowUserPointer(window));
  if (!data) return;

  data->defValue += static_cast<float>(yoffset);
}

static void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) 
{
  AppData* data = reinterpret_cast<AppData*>(glfwGetWindowUserPointer(window));
  if (!data) return;

  if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
    if (action == GLFW_PRESS) {
      data->isLeftMouseDown = true;
      glfwGetCursorPos(window, &data->mouseX, &data->mouseY);
    } else if (action == GLFW_RELEASE) {
      data->isLeftMouseDown = false;

      data->offsetX += data->dx;
      data->offsetY += data->dy;

      data->dx = 0;
      data->dy = 0;
    }
  }
}

static void glfwCursorPosCallback(GLFWwindow* window, double xpos, double ypos) 
{
  AppData* data = reinterpret_cast<AppData*>(glfwGetWindowUserPointer(window));
  if (!data) return;

  if (data->isLeftMouseDown) {
    data->dx = xpos - data->mouseX;
    data->dy = ypos - data->mouseY;

    data->totalX = data->offsetX + data->dx;
    data->totalY = data->offsetY + data->dy;
  }
}

static void Init(AppData& data, GLFWwindow* window)
{
  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK) {
    std::cerr << "GLEW init failed." << std::endl;
    exit(EXIT_FAILURE);
  }
  printOpenGLVersionInfo();

  data.renderingProgram = createShaderProgram(data, 0);
  initializeUniforms(data);

  glGenVertexArrays(NUM_VAOS, data.vao);
  glBindVertexArray(data.vao[0]);

  glfwSwapInterval(1);
  glfwSetKeyCallback(window, glfwKeyCallback);
  glfwSetScrollCallback(window, glfwScrollCallback);
  glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);
  glfwSetCursorPosCallback(window, glfwCursorPosCallback);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); 
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 460");
}

static void Draw(AppData& data, GLFWwindow* window, double currentTime)
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(data.renderingProgram);

  data.x += data.inc;
  data.t += data.tinc;

  glProgramUniform1f(data.renderingProgram, data.uniforms.offset, data.x);
  glProgramUniform1f(data.renderingProgram, data.uniforms.time, data.t);
  glProgramUniform1i(data.renderingProgram, data.uniforms.width, data.width);
  glProgramUniform1i(data.renderingProgram, data.uniforms.height, data.height);
  glProgramUniform1i(data.renderingProgram, data.uniforms.type, data.menuCase);
  glProgramUniform1f(data.renderingProgram, data.uniforms.pos, data.defValue);
  glProgramUniform1i(data.renderingProgram, data.uniforms.rotate, data.defRotationSpeed);
  glProgramUniform1i(data.renderingProgram, data.uniforms.render, data.defRenderAmount);
  glProgramUniform1f(data.renderingProgram, data.uniforms.mouseX, data.totalX);
  glProgramUniform1f(data.renderingProgram, data.uniforms.mouseY, data.totalY);

  glDrawArrays(GL_TRIANGLES, 0, 3);
}

static void ImGuiDraw(AppData& data, GLFWwindow* window) 
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
    ImGui::OpenPopup("context_menu");
  }

  if (ImGui::BeginPopup("context_menu")) {
    if (ImGui::MenuItem("Cube"))          { data.menuCase = 0; }
    if (ImGui::MenuItem("Trimmed Cubes")) { data.menuCase = 1; }
    if (ImGui::MenuItem("Cubes"))         { data.menuCase = 2; }
    if (ImGui::MenuItem("Spheres"))       { data.menuCase = 3; }
    if (ImGui::MenuItem("Exit")) {
      exit(EXIT_SUCCESS);
    }
    ImGui::EndPopup();
  }

  switch (data.menuCase) {
  case 0:
    ImGui::SliderInt("RenderSteps",    &data.defRenderAmount, 1, 1500);
    ImGui::SliderFloat("Distance",       &data.defValue,       -10, 50);
    ImGui::SliderInt("Rotation speed", &data.defRotationSpeed, 1, 16);
    ImGui::SliderFloat("Time", &data.t, 0, 2000000);
    break;
  case 1:
    ImGui::SliderInt("RenderSteps", &data.defRenderAmount, 1, 1500);
    ImGui::SliderFloat("zSpeed",      &data.defValue,       -10, 20);
    break;
  case 2:
    ImGui::SliderInt("RenderSteps",    &data.defRenderAmount, 1, 1500);
    ImGui::SliderFloat("Sphere zMove",   &data.defValue,       -10, 200);
    break;
  case 3:
    ImGui::SliderInt("RenderSteps", &data.defRenderAmount, 1, 1500);
    ImGui::SliderFloat("Time", &data.t, 0, 50000000);
    break;
  default:
    break;
  }

  ImGui::Text("Mouse offset: (%.1f, %.1f)", data.dx, data.dy);
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

  ImGui::Render();
  int display_w, display_h;
  glfwGetFramebufferSize(window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void error_callback(int error, const char *description) {
  fprintf(stderr, "GLFW Error: %s\n", description);
  exit(1);
}

int main(void) 
{
  std::cout << "Initializing GLFW..." << std::endl;
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }

  std::cout << "Initializing GLEW..." << std::endl;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  std::cout << "Creating GLFW window..." << std::endl;
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "3D Scene", glfwGetPrimaryMonitor(), nullptr);
  if (!window) {
    glfwTerminate();
    return EXIT_FAILURE;
  }

  AppData data{};
  data.width            = WIDTH;
  data.height           = HEIGHT;
  data.vertexPath       = VPATH;
  data.fragmentPath     = FPATH;
  data.menuCase         = 0;
  data.defValue         = 0;
  data.defRotationSpeed = 12;
  data.defRenderAmount  = 80;
  data.x                = 0.0f;
  data.t                = 0.0f;
  data.inc              = 0.01f;
  data.tinc             = 0.02f;
  data.isLeftCtrlDown   = false;
  data.isLeftMouseDown  = false;
  data.mouseX           = 0.0f;
  data.mouseY           = 0.0f;
  data.dx               = 0.0f;
  data.dy               = 0.0f;
  data.offsetX          = 0.0f;
  data.offsetY          = 0.0f;

  glfwSetWindowUserPointer(window, &data);

  Init(data, window);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    double currentTime = glfwGetTime();
    Draw(data, window, currentTime);
    ImGuiDraw(data, window);

    glfwSwapBuffers(window);
  }

  windowTerminate(window);
  return 0;
}

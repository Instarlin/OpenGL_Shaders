#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <functional>
#include <windows.h>
#include <imgui/backend/imgui_impl_glfw.h>
#include <imgui/backend/imgui_impl_opengl3.h>
#include <Credentials.h>

using namespace std;
using KeyCallback = std::function<void(GLFWwindow*, int, int, int, int)>;

#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#define numVAOs 1

const int width = 1920, height = 1080;
const char* vertexPath = VPATH;
const char* fragmentPath = FPATH;
GLuint renderingProgram, fShader, vShader, vao[numVAOs];
int menuCase = 0, defvalue = 0, defRotationSpeed = 12, defRenderAmount = 80;
float x = 0.0f, t = 0.0f, inc = 0.01f, tinc = 0.02f;
bool is_1_down, is_2_down, is_3_down, is_left_ctrl_down, is_r_down;

string getSystemPowerInfo() {
    SYSTEM_POWER_STATUS sps;
    if(!GetSystemPowerStatus(&sps)) return "Problems with detecting power info";

    switch (sps.BatteryFlag) {
    case 1:
        return "High level";
    case 2:
        return "Low level";
    case 4:
        return "Critical level";
    case 8:
        return "Charging";
    case 128:
        return "No battery";
    case 255:
    default:
        return "Unknown status";
    }
};

void getOpenGLVersionInfo() {
    cout << "Vendor: " << glGetString(GL_VENDOR) << endl;
    cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
    cout << "Version: " << glGetString(GL_VERSION) << endl;
    cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
    cout << "Battery status: " << getSystemPowerInfo() << endl;
};

void windowTerminate(GLFWwindow* window) {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
};

string readShaderSource(const char* filePath, const char* type) {
    string content;
    ifstream fileStream(filePath);
    if (!fileStream.is_open()) {cout << "Failed to open " << type << " shader" << endl;}
    string line = "";
    while (!fileStream.eof()) {
        getline(fileStream, line);
        content.append(line + "\n");
    }
    fileStream.close();
    return content;
};

GLuint createShader(const char* glslCode, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, (const char**)&glslCode, NULL);
    glCompileShader(shader);   
    return shader;
}

void printProgramLog(int prog) {
    int len = 0;
    int chWrittn = 0;
    char* log;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = (char*)malloc(len);
        glGetProgramInfoLog(prog, len, &chWrittn, log);
        cout << "Program Info Log: " << log << endl;
        free(log);
    }
};

GLuint createShaderProgram(int resetShader) {
    if (resetShader == 1) glDeleteProgram(renderingProgram);
    string vertexShaderStr = readShaderSource(vertexPath, "Vertex");
    string fragmentShaderStr = readShaderSource(fragmentPath, "Fragment");
    const char* vertexShader = vertexShaderStr.c_str();
    const char* fshaderSource = fragmentShaderStr.c_str();

    vShader = createShader(vertexShader, GL_VERTEX_SHADER);
    fShader = createShader(fshaderSource, GL_FRAGMENT_SHADER);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vShader);
    glAttachShader(shaderProgram, fShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vShader);
    glDeleteShader(fShader);

    GLint result;
    glGetShaderiv(shaderProgram, GL_LINK_STATUS, &result);
    if (result != 1) {
        printProgramLog(shaderProgram);
    }

    return shaderProgram;
};

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_LEFT_CONTROL) is_left_ctrl_down = action == GLFW_PRESS;
  if (is_left_ctrl_down) {
    switch (key) {
    case GLFW_KEY_R:
      renderingProgram = createShaderProgram(1);
      break;
    case GLFW_KEY_1:
      menuCase = 0;
      break;
    case GLFW_KEY_2:
      menuCase = 1;
      break;
    case GLFW_KEY_3:
      menuCase = 2;
      break;
    case GLFW_KEY_4:
      menuCase = 3;
    }
  };
};

void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
  defvalue += (int)yoffset;
};

void Init(GLFWwindow* window) {
  glfwMakeContextCurrent(window);
  if (glewInit() != GLEW_OK) exit(EXIT_FAILURE);
  getOpenGLVersionInfo();

  renderingProgram = createShaderProgram(0);
  glGenVertexArrays(numVAOs, vao);
  glBindVertexArray(vao[0]);
  glfwSwapInterval(1);
  glfwSetKeyCallback(window, glfwKeyCallback);
  glfwSetScrollCallback(window, glfwScrollCallback);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 460");
};

void Draw(GLFWwindow* window, double currentTime) {
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(renderingProgram);

  x += inc;
  t += tinc;
  GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offset");
  glProgramUniform1f(renderingProgram, offsetLoc, x);
  GLuint timeLoc = glGetUniformLocation(renderingProgram, "time");
  glProgramUniform1f(renderingProgram, timeLoc, t);
  GLuint widthLoc = glGetUniformLocation(renderingProgram, "width");
  glProgramUniform1i(renderingProgram, widthLoc, width);
  GLuint heightLoc = glGetUniformLocation(renderingProgram, "height");
  glProgramUniform1i(renderingProgram, heightLoc, height);
  GLuint typeLoc = glGetUniformLocation(renderingProgram, "type");
  glProgramUniform1i(renderingProgram, typeLoc, menuCase);
  GLuint posLoc = glGetUniformLocation(renderingProgram, "posOffset");
  glProgramUniform1i(renderingProgram, posLoc, defvalue);
  GLuint rotateLoc = glGetUniformLocation(renderingProgram, "rotateSpeed");
  glProgramUniform1i(renderingProgram, rotateLoc, defRotationSpeed);
  GLuint renderLoc = glGetUniformLocation(renderingProgram, "renderSteps");
  glProgramUniform1i(renderingProgram, renderLoc, defRenderAmount);

  glDrawArrays(GL_TRIANGLES, 0, 3);
};

void ImGuiDraw(GLFWwindow* window) {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) ImGui::OpenPopup("context_menu");
  if (ImGui::BeginPopup("context_menu")) {
    if (ImGui::MenuItem("Cube")) {
      menuCase = 0;
    }
    if (ImGui::MenuItem("Trimmed Cubes")) { 
      menuCase = 1;
    }
    if (ImGui::MenuItem("Cubes")) {
      menuCase = 2;
    }
    if (ImGui::MenuItem("Spheres")) {
      menuCase = 3;
    }
    if (ImGui::MenuItem("Exit")) {
      exit(EXIT_SUCCESS);
    }
    ImGui::EndPopup();
  };

  switch (menuCase) {
    case 0:
      ImGui::SliderInt("RenderSteps", &defRenderAmount, 1, 1500);
      ImGui::SliderInt("Distance", &defvalue, -10, 20);
      ImGui::SliderInt("Rotation speed", &defRotationSpeed, 1, 16);
      break;
    case 1:
      ImGui::SliderInt("RenderSteps", &defRenderAmount, 1, 1500);
      ImGui::SliderInt("zSpeed", &defvalue, -10, 20);
      break;
    case 2:
      ImGui::SliderInt("RenderSteps", &defRenderAmount, 1, 1500);
      ImGui::SliderInt("Sphere zMove", &defvalue, -10, 200);
      break;
    case 3:
      ImGui::SliderInt("RenderSteps", &defRenderAmount, 1, 1500);
    default:
      break;
  };
  ImGui::Text("Fps: %.3f", ImGui::GetIO().Framerate);

  ImGui::Render();
  int display_w, display_h;
  glfwGetFramebufferSize(window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
};

int main(void) {
  if (!glfwInit()) exit(EXIT_FAILURE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_RESIZABLE, true);
  GLFWwindow* window = glfwCreateWindow(width, height, "3D Scene", glfwGetPrimaryMonitor(), NULL);

  Init(window);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    Draw(window, glfwGetTime());
    ImGuiDraw(window);

    glfwSwapBuffers(window);
  };

  windowTerminate(window);
};
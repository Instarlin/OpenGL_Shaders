#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <vector>
#include <Credentials.h>

using namespace std;

const int width = 750, height = 550, icow = 32, icoh = 32;

const char *vertexPath = VPATH;
const char *fragmentPath = FPATH;
const char *icoPath = IPATH;

string getSystemPowerInfo() {
  BOOL WINAPI GetSystemPowerStatus(LPSYSTEM_POWER_STATUS lpSystemPowerStatus);

  SYSTEM_POWER_STATUS sps;
  if(!GetSystemPowerStatus(&sps)) return "Problems with detecting power info";

  switch (sps.BatteryFlag) {
  case 1:
    return "High level";
    break;
  case 2:
    return "Low level";
    break;
  case 4:
    return "Critical level";
    break;
  case 8:
    return "Charging";
    break;
  case 128:
    return "No battery";
    break;
  case 255:
  default:
    return "Unknown status";
    break;
  }

  switch (sps.ACLineStatus) {
  case 0:
    break;
  case 1:
    break;
  case 255:
  default:
    break;
  }
};

void getOpenGLVersionInfo() {
  cout << "Vendor: " << glGetString(GL_VENDOR) << endl;
  cout << "Rerenderer: " << glGetString(GL_RENDERER) << endl;
  cout << "Version: " << glGetString(GL_VERSION) << endl;
  cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
  cout << "Battery status: " << getSystemPowerInfo() << endl;
};

#define numVAOs 1
GLuint renderingProgram;
GLuint fShader;
GLuint vShader;
GLuint vao[numVAOs];

string readShaderSource(const char *filePath, const char *type) {
  string content;
  ifstream fileStream(filePath);
  if (!fileStream.is_open()) {cout << "Failed to open " << type << " shader" << endl;};
  string line = "";
  while (!fileStream.eof()) {
    getline(fileStream, line);
    content.append(line + "\n");
  };
  fileStream.close();
  return content;
};

GLuint createShader(const char *glslCode, GLenum type) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, (const char**)&glslCode, NULL);
  glCompileShader(shader);   
  return shader;
};

void printProgramLog(int prog) {
  int len = 0;
  int chWrittn = 0;
  char *log;
  glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
  if (len > 0) {
    log = (char *)malloc(len);
    glGetProgramInfoLog(prog, len, &chWrittn, log);
    cout << "Program Info Log: " << log << endl;
    free(log);
  };
};

GLuint createShaderProgram(int resetShader) {
  if (resetShader == 1) glDeleteProgram(renderingProgram);
  string vertexShaderStr = readShaderSource(vertexPath, "Vertex");
  string fragmentShaderStr = readShaderSource(fragmentPath, "Fragment");
  const char *vertexShader = vertexShaderStr.c_str();
  const char *fshaderSource = fragmentShaderStr.c_str();

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
  };

  return shaderProgram;
};

void Init(GLFWwindow* window) {
  getOpenGLVersionInfo();
  renderingProgram = createShaderProgram(0);
  glGenVertexArrays(numVAOs, vao);
  glBindVertexArray(vao[0]);
};

float x = 0.0f, t = 0.0f, inc = 0.01f, tinc = 0.02f;

void display(GLFWwindow* window, double currentTime) {
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

  glDrawArrays(GL_TRIANGLES, 0, 3);
};

bool is_left_ctrl_down = false;
bool is_r_down = false;

void RefreshPressed(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_LEFT_CONTROL) is_left_ctrl_down = action == GLFW_PRESS;
  if (key == GLFW_KEY_R) is_r_down = action == GLFW_PRESS;
  if (is_left_ctrl_down && is_r_down) {
    renderingProgram = createShaderProgram(1);
  };
};

int main(void) {
  if (!glfwInit()) exit(EXIT_FAILURE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_RESIZABLE, true);
  GLFWwindow* window = glfwCreateWindow(width, height, "ラストエグザイル", NULL, NULL);
  //* glfwGetPrimaryMonitor() in order to get fullscreen
  glfwMakeContextCurrent(window);
  if (glewInit() != GLEW_OK) exit(EXIT_FAILURE);
  glfwSwapInterval(1);
  Init(window);
  while (!glfwWindowShouldClose(window)) {
    display(window, glfwGetTime());
    glfwSwapBuffers(window);
    glfwSwapInterval(1);
    glfwPollEvents();
    glfwSetKeyCallback(window, RefreshPressed);
  };
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
};

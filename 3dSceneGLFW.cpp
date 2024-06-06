#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <vector>

using namespace std;

const int width = 750, height = 550;

const char *vertexPath = "C:/instarlin/programming/three js/projects/src/glsl/vertex.glsl";
const char *fragmentPath = "C:/instarlin/programming/three js/projects/src/glsl/noiseCloud.glsl";

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

void GetOpenGLVersionInfo() {
  cout << "Vendor: " << glGetString(GL_VENDOR) << endl;
  cout << "Rerenderer: " << glGetString(GL_RENDERER) << endl;
  cout << "Version: " << glGetString(GL_VERSION) << endl;
  cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
  cout << "Battery status: " << getSystemPowerInfo() << endl;
};

#define numVAOs 1
GLuint renderingProgram;
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

GLuint createShaderProgram() {
  string vertexShaderStr = readShaderSource(vertexPath, "Vertex");
  string fragmentShaderStr = readShaderSource(fragmentPath, "Fragment");
  const char *vertexShader = vertexShaderStr.c_str();
  const char *fshaderSource = fragmentShaderStr.c_str();

  GLuint vShader = createShader(vertexShader, GL_VERTEX_SHADER);
  GLuint fShader = createShader(fshaderSource, GL_FRAGMENT_SHADER);

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vShader);
  glAttachShader(shaderProgram, fShader);
  glLinkProgram(shaderProgram);

  GLint result;
  glGetShaderiv(shaderProgram, GL_LINK_STATUS, &result);
  if (result != 1) {
    printProgramLog(shaderProgram);
  };

  return shaderProgram;
};

void Init(GLFWwindow* window) {
  GetOpenGLVersionInfo();
  renderingProgram = createShaderProgram();
  glGenVertexArrays(numVAOs, vao);
  glBindVertexArray(vao[0]);
};

float x = 0.0f, t = 0.0f, inc = 0.01f, tinc = 0.02f;
vector<int> arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

void Swap(int &a, int &b) {
  int temp = a;
  a = b;
  b = temp;
};

int fisherYatesShuffle(vector<int> &arr) {
  int n = arr.size();
  srand(time(0));

  for (int i = n - 1; i > 0; --i) {
    int j = rand() % (i + 1);
    Swap(arr[i], arr[j]);
  };

  int res = 0;
  for (int num : arr) {
    res += res * 10 + num;
  }

  return res;
};

int newHash = fisherYatesShuffle(arr);

void Display(GLFWwindow* window, double currentTime) {
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
  GLuint hashLoc = glGetUniformLocation(renderingProgram, "hash");
  glProgramUniform1i(renderingProgram, hashLoc, newHash);

  glDrawArrays(GL_TRIANGLES, 0, 3);
};

int main(void) {
  if (!glfwInit()) exit(EXIT_FAILURE); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  GLFWwindow* window = glfwCreateWindow(width, height, "SceneGLFW", NULL, NULL);
  //* glfwGetPrimaryMonitor() in order to get fullscreen
  glfwMakeContextCurrent(window);
  if (glewInit() != GLEW_OK) exit(EXIT_FAILURE);
  glfwSwapInterval(1);
  Init(window);
  while (!glfwWindowShouldClose(window)) {
    Display(window, glfwGetTime());
    glfwSwapBuffers(window);
    glfwPollEvents();
  };
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
};

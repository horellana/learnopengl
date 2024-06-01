#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define MAX_FILE_SIZE 8192

char *SHADER_FILES[] = {
  "shaders/triangle.vert",
};

int read_file(char *file_path, char *output, size_t output_size) {

  FILE *file = fopen(file_path, "r");

  if (!file) {
    return -1;
  }

  size_t bytes_read = fread(output, 1, output_size - 1, file);

  if (ferror(file)) {
    return -1;
  }

  output[bytes_read] = '\0';

  fclose(file);
}

unsigned int load_shader(char *input, GLenum shader_type) {
  char shader_source[MAX_FILE_SIZE] = { 0 };
  int read_shader_error = read_file(input, shader_source, sizeof(shader_source));

  if (read_shader_error != 0) {
    perror("Failed to load shader source");
  }

  unsigned int shader = glCreateShader(shader_type);

  if (shader == 0) {
    fprintf(stderr, "Failed to create shader\n");
  }

  const char *src = shader_source;
  glShaderSource(shader, 1, &src, NULL);

  glCompileShader(shader);

  int  success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    fprintf(stderr, "%s\n", infoLog);
  }
}

unsigned int create_shader_program(unsigned int *shaders, size_t shaders_size) {
  unsigned int shader_program = glCreateProgram();

  for (size_t i = 0; i < shaders_size; i++) {
    glAttachShader(shaders[i], shader_program);
  }

  glLinkProgram(shader_program);
  glUseProgram(shader_program);

  for (size_t i = 0; i < shaders_size; i++) {
    glDeleteShader(shaders[i]);
  }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, 1);
  }
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    fprintf(stderr, "Failed to create GLFW window\n");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD");
    return -1;
  }

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f,  0.5f, 0.0f
  };

  unsigned int vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  unsigned int vertex_shader = load_shader("shaders/triangle.vert", GL_VERTEX_SHADER);
  unsigned int fragment_shader = load_shader("shaders/triangle.frag", GL_FRAGMENT_SHADER);

  /* unsigned int shader_program = glCreateProgram(); */

  /* glAttachShader(shader_program, vertex_shader); */
  /* glAttachShader(shader_program, fragment_shader); */
  /* glLinkProgram(shader_program); */

  /* glUseProgram(shader_program); */

  /* glDeleteShader(vertex_shader); */
  /* glDeleteShader(fragment_shader); */

  while(!glfwWindowShouldClose(window)) {
    process_input(window);

    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}

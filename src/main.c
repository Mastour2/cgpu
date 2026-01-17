#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

// Define Values
#define WIDTH 1024
#define HEIGHT 620

const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";




// Global Variables
GLFWwindow* window;

// Window Functions
void window_init();
void window_create();
void window_process_input(GLFWwindow *window);
void window_frame_resize(GLFWwindow* _window, int width, int height);

// utils

// Shader Functions
char* shader_load_source_file(const char* path);
char* shader_compile(const char*, unsigned int type);
unsigned int shader_create_program(char* vertex, char* fragment);

// Buffer Functions


// Entry
int main(void) {
    window_init();
    window_create();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, window_frame_resize);


    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };


    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char info[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, info);
        glDeleteShader(vertexShader);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED: %s\n", info);
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);


    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, info);
        glDeleteShader(fragmentShader);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: %s\n", info);
    }


    unsigned int program;
    program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, info);
        glDeleteProgram(program);
        printf("ERROR::SHADER::PROGRAM::LINK_FAILED: %s\n", info);
    }

    glUseProgram(program);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    unsigned int vbo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        window_process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);


        // swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // de-allocate all resources
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(program);


    // clearing all previously allocated GLFW resources
    glfwTerminate();
    return 0;
}


void window_init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
}

void window_create() {
    window = glfwCreateWindow(WIDTH, HEIGHT, "GLFW", NULL, NULL);
    if (!window) glfwTerminate();

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (monitor != NULL) {
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowPos(window, (mode->width - WIDTH) / 2, (mode->height - HEIGHT) / 2);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwShowWindow(window);
}

void window_frame_resize(GLFWwindow* _window, int width, int height) {
    glViewport(0, 0, width, height);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwSwapBuffers(window);
    printf("Framebuffer size changed to: width %d height %d\n", width, height);
}

void window_process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}


char* shader_load_source_file(const char* filename) {
    FILE* file = fopen(filename, "br");

    if (!file) {
        printf("Could not open file: %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(len + 1);
    fread(buffer, 1, len, file);
    buffer[len] = '\0';

    fclose(file);
    return buffer;
}

char* shader_compile(const char*, unsigned int type) {}

unsigned int shader_create_program(char* vertex, char* fragment) {}

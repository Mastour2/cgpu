#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define WIDTH 1024
#define HEIGHT 620

// Function declarations
static inline void window_init();
static inline void window_create();
static inline void window_process_input(GLFWwindow *p_window);
static inline void window_frame_resize(GLFWwindow* p_window, int width, int height);

// Function implementation
#ifdef WINDOW_IMPLEMENTATION
    #include <stdio.h>

    GLFWwindow* window;

    static inline void window_init() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    }

    static inline void window_create() {
         window = glfwCreateWindow(WIDTH, HEIGHT, "GLFW", NULL, NULL);
        if (!window) glfwTerminate();

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        if (monitor != NULL) {
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            glfwSetWindowPos(window, (mode->width - WIDTH) / 2, (mode->height - HEIGHT) / 2);
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        glfwShowWindow(window);

        glfwSetFramebufferSizeCallback(window, window_frame_resize);
    }

    static inline void window_frame_resize(GLFWwindow* p_window __attribute__((unused)), int width, int height) {
        // (void)p_window;

        glViewport(0, 0, width, height);
        glClearColor(0.35f, 0.35f, 0.35f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);
        printf("Framebuffer size changed to: width %d height %d\n", width, height);
    }

    static inline void window_process_input(GLFWwindow *p_window) {
        if (glfwGetKey(p_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(p_window, GL_TRUE);
        }
    }
#endif

#endif

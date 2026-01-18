#include <glad/glad.h>
#include <vcruntime.h>
#define WINDOW_IMPLEMENTATION
#include <window.h>

#define SHADER_IMPLEMENTATION
#include <shader.h>

#define VERTEX_PATH "C:/Users/i5lot/Documents/GraphicApi/OpenGL-GLFW/src/shaders/default.vert"
#define FRAGMENT_PATH "C:/Users/i5lot/Documents/GraphicApi/OpenGL-GLFW/src/shaders/default.frag"

#define FRAGMENT_PATH_2 "C:/Users/i5lot/Documents/GraphicApi/OpenGL-GLFW/src/shaders/red.frag"

//
void buffer_create_vao(unsigned int* p_vao);
void buffer_create_vbo(unsigned int* p_vbo, float* vertices, size_t size);
void buffer_create_ebo(unsigned int* p_ebo, unsigned int* indices, size_t size);
// DATA

float tri_vertices[] = {
      0.0f,   0.5f, 0.0f,   // top
     -0.5f, -0.5f, 0.0f,    // left
      0.5f,  -0.5f, 0.0f    // rigth
};

float rect_vertices[] = {
    0.5f,  0.5f, 0.0f,  // top right
    0.5f, -0.5f, 0.0f,  // bottom right
   -0.5f, -0.5f, 0.0f,  // bottom left
   -0.5f,  0.5f, 0.0f   // top left
};

unsigned int rect_indices[] = {
    0, 1, 3,
    1, 2, 3
};

// Entry
int main(void) {
    window_init();
    window_create();

    unsigned int program = shader_create(VERTEX_PATH, FRAGMENT_PATH);
    unsigned int program_2 = shader_create(VERTEX_PATH, FRAGMENT_PATH_2);

    // unsigned int vbos[2], vaos[2], ebo;
    // glGenVertexArrays(2, vaos); // generate multiple VAOs Buffer
    // glGenBuffers(2, vbos); // generate multiple VBOs Buffer
    // glGenBuffers(1, &ebo);

    // Rectangle
    unsigned int rect_vbo, rect_vao, rect_ebo;
    buffer_create_vao(&rect_vao);
    buffer_create_vbo(&rect_vbo, rect_vertices, sizeof(rect_vertices));
    buffer_create_ebo(&rect_ebo, rect_indices, sizeof(rect_indices));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Triangle
    unsigned int tri_vbo, tri_vao;
    buffer_create_vao(&tri_vao);
    buffer_create_vbo(&tri_vbo, tri_vertices, sizeof(tri_vertices));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window)) {
        window_process_input(window);

        glClearColor(0.35f, 0.35f, 0.35f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(rect_vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

        glUseProgram(program_2);
        glBindVertexArray(tri_vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        // swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // de-allocate all resources
    glDeleteVertexArrays(1, &rect_vao);
    glDeleteBuffers(1, &rect_vbo);
    glDeleteBuffers(1, &rect_ebo);

    glDeleteVertexArrays(1, &tri_vao);
    glDeleteBuffers(1, &tri_vbo);


    glDeleteProgram(program);
    glDeleteProgram(program_2);

    // clearing all previously allocated GLFW resources
    glfwTerminate();
    return 0;
}


void buffer_create_vao(unsigned int* p_vao) {
    glGenVertexArrays(1, p_vao);
    glBindVertexArray(*p_vao);
}

void buffer_create_vbo(unsigned int* p_vbo, float* vertices, size_t size) {
    glGenBuffers(1, p_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, *p_vbo);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void buffer_create_ebo(unsigned int* p_ebo, unsigned int* indices, size_t size) {
    glGenBuffers(1, p_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *p_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

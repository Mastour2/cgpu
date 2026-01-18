#include <glad/glad.h>
#include <cglm/cglm.h>

#define WINDOW_IMPLEMENTATION
#include <window.h>

#define SHADER_IMPLEMENTATION
#include <shader.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERTEX_PATH "C:/Users/i5lot/Documents/GraphicApi/OpenGL-GLFW/src/shaders/default.vert"
#define FRAGMENT_PATH "C:/Users/i5lot/Documents/GraphicApi/OpenGL-GLFW/src/shaders/default.frag"

#define FRAGMENT_PATH_2 "C:/Users/i5lot/Documents/GraphicApi/OpenGL-GLFW/src/shaders/red.frag"

// (CPU side) [Position (3f), Normal (3f), UV (2f)]
typedef struct {
    float *positions, *normals, *uvs;
    unsigned int *indices;
    unsigned int vertex_count, index_count;
} geometry_t;

// (GPU side) [P1, P2, P3, N1, N2, N3, UV1, UV2...]
typedef struct {
    unsigned int vao, vbo, ebo;
    unsigned int vertex_count, index_count;
    mat4 model;
} mesh_t; // model

typedef struct {
    uint8_t r, g, b;
    } color_t;

#define GRAY (color_t){ 120, 120, 120 }

// Buffer
void buffer_create_vao(unsigned int* p_vao);
void buffer_create_vbo(unsigned int* p_vbo, float* vertices, size_t size);
void buffer_create_ebo(unsigned int* p_ebo, unsigned int* indices, size_t size);
void buffer_destroy_geometry(mesh_t* mesh);
mesh_t buffer_create_geometry(geometry_t geo);
// Renderer
void draw_command(mesh_t* mesh);
void draw_mesh(mesh_t* mesh, int* uniforms);
void clear(color_t color);


geometry_t geometry_create_rect() {
    geometry_t geo = {0};

    geo.vertex_count = 4;
    geo.index_count = 6;

    geo.positions = (float *)malloc(geo.vertex_count * 3 * sizeof(float));
    geo.normals = (float *)malloc(geo.vertex_count * 3 * sizeof(float));
    geo.uvs = (float *)malloc(geo.vertex_count * 2 * sizeof(float));
    geo.indices = (unsigned int *)malloc(geo.index_count * sizeof(unsigned int));

    float positions[] = {
         0.5f,  0.5f, 0.0f, // top right
         0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f  // top left
    };

    memcpy(geo.positions, positions, sizeof(positions));

    float normals[] = {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f
    };

    memcpy(geo.normals, normals, sizeof(normals));

    float uvs[] = {
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f
    };

    memcpy(geo.uvs, uvs, sizeof(uvs));

    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2
    };

    memcpy(geo.indices, indices, sizeof(indices));

    return geo;
}

geometry_t geometry_create_triangle() {
    geometry_t geo = {0};


    geo.vertex_count = 4;
    geo.index_count = 0;

    geo.positions = (float *)malloc(geo.vertex_count * 3 * sizeof(float));
    geo.normals = (float *)malloc(geo.vertex_count * 3 * sizeof(float));
    geo.uvs = (float *)malloc(geo.vertex_count * 2 * sizeof(float));
    geo.indices = 0;


    float positions[] =  {
         0.0f,   0.5f,  0.0f,
        -0.5f,  -0.5f,  0.0f,
         0.5f,  -0.5f,  0.0f
    };

    memcpy(geo.positions, positions, sizeof(positions));

    float normals[] = {
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };

    memcpy(geo.normals, normals, sizeof(normals));

    float uvs[] = {
        0.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 0.0f
    };

    memcpy(geo.uvs, uvs, sizeof(uvs));

    return geo;
}


void geometry_free(geometry_t* geo) {
    free(geo->positions);
    free(geo->normals);
    free(geo->uvs);
    free(geo->indices);
}



mesh_t rect() {
    geometry_t geo = geometry_create_rect();
    mesh_t mesh = buffer_create_geometry(geo);

    geometry_free(&geo);

    return mesh;
}


mesh_t triangle() {
    geometry_t geo = geometry_create_triangle();
    mesh_t mesh = buffer_create_geometry(geo);

    geometry_free(&geo);

    return mesh;
}


// Entry
int main(void) {
    window_init();
    window_create();

    unsigned int program = shader_create(VERTEX_PATH, FRAGMENT_PATH);
    unsigned int program_2 = shader_create(VERTEX_PATH, FRAGMENT_PATH_2);

    int uniforms[] = {
        glGetUniformLocation(program, "model")
    };

    // TRIANGLE
    mesh_t tri_mesh = triangle();
    // RECT 1
    mesh_t rect_mesh = rect();;
    // RECT 2
    mesh_t rect2_mesh = rect();

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    printf("Maximum nr of vertex attributes supported: %d\n", nrAttributes);

    while (!glfwWindowShouldClose(window)) {
        window_process_input(window);

        clear(GRAY);

        glUseProgram(program_2);
        draw_mesh(&rect_mesh, uniforms);

        glUseProgram(program);
        draw_mesh(&tri_mesh, uniforms);

        glBindVertexArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /*  de-allocate all resources  */

    buffer_destroy_geometry(&rect_mesh);
    buffer_destroy_geometry(&rect2_mesh);
    buffer_destroy_geometry(&tri_mesh);

    glDeleteProgram(program);
    glDeleteProgram(program_2);
    // clearing all previously allocated GLFW resources
    glfwTerminate();
    return 0;
}


void clear(color_t color) {
    glClearColor(
        (float)color.r / 255.0f,
        (float)color.g / 255.0f,
        (float)color.b / 255.0f, 1.0f
    );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void draw_command(mesh_t* mesh) {
    glBindVertexArray(mesh->vao);
    if (mesh->ebo > 0) {
        glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, (void*)0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, mesh->vertex_count);
    }
    glBindVertexArray(0);
}

void draw_mesh(mesh_t* mesh, int* uniforms) {
    glm_mat4_identity(mesh->model);
    glm_translate(mesh->model, (vec3){0.0f, 0.0f, -1.0f});

    int modelLoc = uniforms[0];

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mesh->model[0][0]);

    draw_command(mesh);
}


//
void buffer_create_vao(unsigned int* p_vao) {
    glGenVertexArrays(1, p_vao);
    glBindVertexArray(*p_vao);
}

//
void buffer_create_vbo(unsigned int* p_vbo, float* vertices, size_t size) {
    glGenBuffers(1, p_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, *p_vbo);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

//
void buffer_create_ebo(unsigned int* p_ebo, unsigned int* indices, size_t size) {
    glGenBuffers(1, p_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *p_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

void buffer_destroy_geometry(mesh_t* mesh) {
    glDeleteVertexArrays(1, &mesh->vao);
    glDeleteBuffers(1, &mesh->vbo);
    glDeleteBuffers(1, &mesh->ebo);
    *mesh = (mesh_t){0};
}

//
mesh_t buffer_create_geometry(geometry_t geo) {
    mesh_t mesh = {0};

    glm_mat4_identity(mesh.model);

    mesh.index_count = geo.index_count;
    mesh.vertex_count = geo.vertex_count;

    // Create vao
    buffer_create_vao(&mesh.vao);

    size_t p_size = geo.vertex_count * 3 * sizeof(float);
    size_t n_size = geo.vertex_count * 3 * sizeof(float);
    size_t u_size = geo.vertex_count * 2 * sizeof(float);

    // Create Buffer and Allocate GPU Memery space
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, p_size + n_size + u_size, NULL, GL_STATIC_DRAW);

    // Upload data
    glBufferSubData(GL_ARRAY_BUFFER, 0, p_size, geo.positions);
    glBufferSubData(GL_ARRAY_BUFFER, p_size, n_size, geo.normals);
    glBufferSubData(GL_ARRAY_BUFFER, p_size + n_size, u_size, geo.uvs);

    // Position: Location 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal: Location 1
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(p_size));
    glEnableVertexAttribArray(1);

    // Uv: Location 2
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(p_size + n_size));
    glEnableVertexAttribArray(2);

    // if not NULL indices, create a ebo buffer for it
    if (geo.indices) {
        buffer_create_ebo(&mesh.ebo, geo.indices, geo.index_count * sizeof(unsigned int));
    }

    glBindVertexArray(0);

    return mesh;
}

//

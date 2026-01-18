#include "./buffers.h"
#include "./geometry.h"

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

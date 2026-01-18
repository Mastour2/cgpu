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


void buffer_create_vao(unsigned int* p_vao);
void buffer_create_vbo(unsigned int* p_vbo, float* vertices, size_t size);
void buffer_create_ebo(unsigned int* p_ebo, unsigned int* indices, size_t size);
void buffer_destroy_geometry(mesh_t* mesh);

mesh_t buffer_create_geometry(geometry_t geo);

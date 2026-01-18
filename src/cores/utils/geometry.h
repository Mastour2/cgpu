// (CPU side) [Position (3f), Normal (3f), UV (2f)]
typedef struct {
    float *positions, *normals, *uvs;
    unsigned int *indices;
    unsigned int vertex_count, index_count;
} geometry_t;

geometry_t geometry_create_triangle();
geometry_t geometry_create_rect();
void geometry_free(geometry_t* geo);

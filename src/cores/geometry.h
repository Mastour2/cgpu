// (CPU side) [Position (3f), Normal (3f), UV (2f)]
typedef struct {
    float *positions;
    float *normals;
    float *uvs;
    unsigned int *indices;
    unsigned int vertex_count;
    unsigned int index_count;
} geometry_t;

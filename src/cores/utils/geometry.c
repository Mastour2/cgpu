#include <string.h>
#include <stdlib.h>
#include "./geometry.h"

// Geometrics
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

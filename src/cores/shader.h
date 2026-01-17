#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

// Function declarations
static inline char* shader_load_source_file(const char* path);
static inline unsigned int shader_compile(const char* shader_source, unsigned int shader_type);
static inline unsigned int shader_create(const char* vertex_path, const char* fragment_path);

// Function implementation
#ifdef SHADER_IMPLEMENTATION
    #include <glad/glad.h>
    #include <stdio.h>
    #include <stdlib.h>

    #define LOG_BUFFER_SIZE 512

    static inline char* shader_load_source_file(const char* path) {
        FILE* file = fopen(path, "rb");

        if (!file) {
            fprintf(stderr, "[IO Error] Could not open file: %s\n", path);
            return NULL;
        }

        fseek(file, 0, SEEK_END);
        long length = ftell(file);
        fseek(file, 0, SEEK_SET);

        char* buffer = (char*)malloc(length + 1);

        if (!buffer) {
            fprintf(stderr, "[Memory Error] Could not allocate buffer for shader.\n");
            fclose(file);
            return NULL;
        }

        fread(buffer, 1, length, file);
        buffer[length] = '\0';

        fclose(file);
        return buffer;
    }

    static inline unsigned int shader_compile(const char* shader_source, unsigned int shader_type) {
        unsigned int shader_id = glCreateShader(shader_type);
        glShaderSource(shader_id, 1, &shader_source, NULL);
        glCompileShader(shader_id);

        int compile_status;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status);

        if (!compile_status) {
            char info_log[LOG_BUFFER_SIZE];
            const char* type_string= (shader_type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";

            glGetShaderInfoLog(shader_id, LOG_BUFFER_SIZE, NULL, info_log);

            fprintf(stderr, "[Graphics Error] %s Shader Compilation Failed:\n%s\n", type_string, info_log);
            glDeleteShader(shader_id);
            shader_id = 0;
        }

        return shader_id;
    }

    static inline unsigned int shader_create(const char* vertex_path, const char* fragment_path) {
        char* vertex_code = shader_load_source_file(vertex_path);
        char* fragment_code = shader_load_source_file(fragment_path);

        if (!vertex_code || !fragment_code) {
            fprintf(stderr, "[Shader Error] Failed to load shader source files.\n");
            free(vertex_code);
            free(fragment_code);
            return 0;
        }

        unsigned int v_shader = shader_compile(vertex_code, GL_VERTEX_SHADER);
        unsigned int f_shader = shader_compile(fragment_code, GL_FRAGMENT_SHADER);

        unsigned int program_id = glCreateProgram();

        glAttachShader(program_id, v_shader);
        glAttachShader(program_id, f_shader);
        glLinkProgram(program_id);

        int link_status;
        glGetProgramiv(program_id, GL_LINK_STATUS, &link_status);

        if (!link_status) {
            char info_log[LOG_BUFFER_SIZE];
            glGetProgramInfoLog(program_id, LOG_BUFFER_SIZE, NULL, info_log);
            fprintf(stderr, "[Graphics Error] Program Linking Failed:\n%s\n", info_log);
            glDeleteProgram(program_id);
            program_id = 0;
        }

        glDetachShader(program_id, v_shader);
        glDetachShader(program_id, f_shader);
        glDeleteShader(v_shader);
        glDeleteShader(f_shader);

        free(vertex_code);
        free(fragment_code);

        return program_id;
    }
#endif

#endif

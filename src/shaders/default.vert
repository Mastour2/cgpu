#version 410 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 Uv;

uniform mat4 model;

in float var;

void main() {
    gl_Position = model * vec4(Position, 1.0);
}

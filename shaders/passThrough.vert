#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 TexCoord;

out vec3 vertex_position;
out vec3 vertex_normal;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main()
{
    vertex_position = vec3(model * vec4(position, 1.0));
    gl_Position =  projection * view * vec4(vertex_position, 1.0f);
    vertex_normal = mat3(transpose(inverse(model))) * normal;
}
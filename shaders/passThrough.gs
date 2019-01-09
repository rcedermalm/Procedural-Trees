#version 150

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 vertex_position[3];
in vec3 vertex_normal[3];

out vec3 FragPosition;
out vec3 FragNormal;

void main()
{
    vec3 edge1 = vertex_position[1] - vertex_position[0];
    vec3 edge2 = vertex_position[2] - vertex_position[0];

    vec3 faceNormal = normalize(cross(edge1, edge2));

    for(int i = 0; i < gl_in.length(); i++)
    {
        gl_Position = gl_in[i].gl_Position;
        FragPosition = vertex_position[i];
        FragNormal = faceNormal;
        EmitVertex();
    }
    EndPrimitive();
}

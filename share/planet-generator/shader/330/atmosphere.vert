#version 330 core

in vec4 att_vertex;

uniform mat4 matrix_M;
uniform mat4 matrix_PV;

out vec2 tex_coord;

void main()
{
  gl_Position = matrix_PV * matrix_M * att_vertex;
  tex_coord = att_vertex.xy;
}

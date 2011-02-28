#version 330 core

in vec4 att_vertex;
in vec4 att_color;

uniform mat4 matrix_M;
uniform mat4 matrix_PV;

out vec4 color;

void main()
{
  gl_Position = matrix_PV * matrix_M * att_vertex;
  color = att_color;
}

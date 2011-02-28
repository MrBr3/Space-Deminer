#version 330 core

in vec4 vertex;
in vec2 att_tex_coord;

uniform mat4 matrix_M;
uniform mat4 matrix_PV;

out vec2 tex_coord;

void main()
{
  gl_Position = matrix_PV * matrix_M * vertex;
  tex_coord = att_tex_coord;
}

#version 330 core

in vec4 att_vertex;
in vec2 att_tex_coord;

uniform mat4 matrix_M;
uniform mat4 matrix_PV;
uniform float ring_uv_factor;

out vec2 tex_coord;
out vec4 world_pos;

void main()
{
  gl_Position = matrix_PV * (world_pos = matrix_M * att_vertex);
  tex_coord = att_tex_coord;
  tex_coord.x *= ring_uv_factor;
}

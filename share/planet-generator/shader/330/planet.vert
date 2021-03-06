#version 330 core

in vec4 vertex;
in vec2 att_tex_coord;
in vec2 att_tex_coord_warped;

uniform mat4 matrix_M;
uniform mat4 matrix_PV;
uniform vec2 uni_seeming_circle_radius;

out vec2 tex_coord;
out vec2 tex_coord_warped;
out vec4 world_pos_;
out vec2 rel_pos;

void main()
{
  gl_Position = matrix_PV * (world_pos_ = matrix_M * vertex);
  tex_coord = att_tex_coord;
  tex_coord_warped = att_tex_coord_warped;
  rel_pos.x = gl_Position.x/(gl_Position.w*uni_seeming_circle_radius.x);
  rel_pos.y = gl_Position.y/(gl_Position.w*uni_seeming_circle_radius.y);
}

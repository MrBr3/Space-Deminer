#version 330 core

in vec4 att_vertex;

uniform mat4 matrix_M;
uniform mat4 matrix_PV;
uniform vec2 uni_seeming_circle_radius;
uniform float uni_outer_radius;

out vec2 rel_pos;

void main()
{
  gl_Position = matrix_PV * matrix_M * att_vertex;
  gl_Position/=gl_Position.w;
  gl_Position.xy *= uni_outer_radius*uni_seeming_circle_radius/abs(gl_Position.xy);
  rel_pos.x = gl_Position.x/(uni_seeming_circle_radius.x);
  rel_pos.y = gl_Position.y/(uni_seeming_circle_radius.y);
}

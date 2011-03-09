#version 330 core

in vec2 rel_pos;

out vec4 resulting_color;

void main()
{
  resulting_color = vec4(rel_pos.x, rel_pos.y, 0., 0.8);
}

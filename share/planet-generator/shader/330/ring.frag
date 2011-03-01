#version 330 core

uniform sampler2D uni_ring_texture;

in vec2 tex_coord;

out vec4 resulting_color;

void main()
{
  if(tex_coord.x>1.)
    discard;
  resulting_color = texture2D(uni_ring_texture, tex_coord);
}

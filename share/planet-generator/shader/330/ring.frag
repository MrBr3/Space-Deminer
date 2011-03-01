#version 330 core

uniform sampler2D uni_ring_texture;

in vec2 tex_coord;

void main()
{
  if(tex_coord.x>1.)
    discard;
  gl_FragColor = texture2D(uni_ring_texture, tex_coord);
}

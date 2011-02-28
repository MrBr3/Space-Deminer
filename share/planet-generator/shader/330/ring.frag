#version 330 core

in vec2 tex_coord;

void main()
{
  if(tex_coord.x>1.)
    discard;
  gl_FragColor = vec4(tex_coord.x, tex_coord.y, 0., 1.);
}

#version 330 core

in vec2 tex_coord;

void main()
{
  gl_FragColor = vec4(tex_coord.x, tex_coord.y, 0., 1.);
}

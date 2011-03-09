#version 330 core

struct Gradient
{
  float slice_id;
  float remap_size, remap_offset;
};

#define GET_GRADIENT_COLOR(g, x) texture1DArray(uni_all_curves, vec2((g.remap_offset+x)*g.remap_size, g.slice_id))

uniform float uni_outer_radius;
uniform sampler1DArray uni_all_curves;
//uniform sampler2D uni_circle_gradient_texture;

in vec2 rel_pos;

out vec4 resulting_color;

void main()
{
  float inv_outer_radius = 1./uni_outer_radius;
  float d = (length(rel_pos)-1.)*1./(uni_outer_radius-1.);

  if(d>1.)
   discard;


  //float d = (texture(uni_circle_gradient_texture, abs(rel_pos)*inv_outer_radius).x-inv_outer_radius)/(1.-inv_outer_radius);
  resulting_color = vec4(d, d, d, 1.);
}

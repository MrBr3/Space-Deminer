#version 330 core

#extension GL_EXT_texture_array: enable

struct Gradient
{
  float slice_id;
  float remap_size, remap_offset;
};

#define GET_GRADIENT_COLOR(g, x) texture1DArray(uni_all_curves, vec2((g.remap_offset+x)*g.remap_size, g.slice_id))

uniform float uni_outer_radius;
uniform sampler1DArray uni_all_curves;
uniform Gradient uni_outer_gradient;
//uniform sampler2D uni_circle_gradient_texture;

in vec2 rel_pos;

out vec4 resulting_color;

void main()
{
  if(rel_pos.x*rel_pos.x+rel_pos.y*rel_pos.y > uni_outer_radius*uni_outer_radius)
   discard;

  float inv_outer_radius = 1./uni_outer_radius;
  float d = (length(rel_pos)-1.)*1./(uni_outer_radius-1.);
  //float d = (texture(uni_circle_gradient_texture, abs(rel_pos)*inv_outer_radius).x-inv_outer_radius)/(1.-inv_outer_radius);

  resulting_color = GET_GRADIENT_COLOR(uni_outer_gradient, d);
}

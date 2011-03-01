#version 330 core

// ==== Texture Layers ========
uniform sampler2D uni_base_texture;
uniform sampler2D uni_night_texture;
uniform sampler2D uni_cloud_texture;
uniform sampler2D uni_weight_texture;

uniform bool uni_just_one_texture_visible;
uniform bool uni_base_texture_visible;
uniform bool uni_base_texture_warped;
uniform bool uni_night_texture_visible;
uniform bool uni_night_texture_warped;
uniform bool uni_cloud_texture_visible;
uniform bool uni_cloud_texture_warped;
uniform bool uni_weight_texture_visible;
uniform bool uni_weight_texture_warped;

#define PLANET_TEXTURE_COLOR(what, t) if(uni_##t##_texture_warped){what texture2D(uni_##t##_texture, tex_coord_warped);}else{what texture2D(uni_##t##_texture, tex_coord);}

in vec2 tex_coord;
in vec2 tex_coord_warped;

// ==== Mateial ========

struct Material
{
  vec4 diffuse;
};

// ============

out vec4 resulting_color;

vec4 query_surface_color();
vec4 just_one_layer();

void main()
{
  if(uni_just_one_texture_visible)
  {
    resulting_color = just_one_layer();
    return;
  }

  vec4 surface_color = query_surface_color();

  resulting_color = surface_color;
}

vec4 just_one_layer()
{
  if(uni_night_texture_visible)
    PLANET_TEXTURE_COLOR(return, night)
  if(uni_cloud_texture_visible)
    PLANET_TEXTURE_COLOR(return, cloud);
  if(uni_weight_texture_visible)
    PLANET_TEXTURE_COLOR(return, weight);
  PLANET_TEXTURE_COLOR(return, base);
}

vec4 query_surface_color()
{
  if(!uni_base_texture_visible)
    return vec4(0.5, 0.5, 0.5, 1.);

  Material mat;

  PLANET_TEXTURE_COLOR(mat.diffuse=, base);

  return mat.diffuse;
}

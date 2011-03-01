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

in vec2 tex_coord;
in vec2 tex_coord_warped;

// ==== Mateial ========

struct Material
{
  vec4 diffuse;
};

// ============

vec4 query_surface_color();
vec4 just_one_layer();

void main()
{
  if(uni_just_one_texture_visible)
  {
    gl_FragColor = just_one_layer();
    return;
  }

  vec4 surface_color = query_surface_color();

  gl_FragColor = surface_color;
}

vec4 just_one_layer()
{
  if(uni_night_texture_visible)
    return texture2D(uni_night_texture, tex_coord);
  if(uni_cloud_texture_visible)
    return texture2D(uni_cloud_texture, tex_coord);
  if(uni_weight_texture_visible)
    return texture2D(uni_weight_texture, tex_coord);
  return texture2D(uni_base_texture, tex_coord);
}

vec4 query_surface_color()
{
  Material mat;

  mat.diffuse = texture2D(uni_base_texture, tex_coord);

  return mat.diffuse;
}

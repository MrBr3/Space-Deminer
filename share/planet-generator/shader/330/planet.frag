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
vec4 query_cloud_color();
vec4 just_one_layer();

void main()
{
  if(uni_just_one_texture_visible)
  {
    resulting_color = just_one_layer();
    return;
  }

  resulting_color = query_surface_color();

  if(uni_cloud_texture_visible)
  {
    float thickness;
    vec4 cloud_color;

    PLANET_TEXTURE_COLOR(cloud_color=, cloud);
    thickness = clamp(max(max(cloud_color.x, cloud_color.y), cloud_color.z), 0.,1.); // TODO use texture with just one component

    cloud_color = query_cloud_color();
    
    resulting_color = mix(resulting_color, cloud_color, thickness);
  }
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

vec4 query_cloud_color()
{
  Material mat;

  mat.diffuse=vec4(1., 1., 1., 1.);

  return mat.diffuse;
}

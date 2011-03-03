#version 330 core

const float PI = 3.14159265358979324;
const float PI2 = 6.28318530717958648;
const float INV_PI = 0.318309886183790672;
const float INV_PI2 = 0.159154943091895336;

float max_vec3(vec3 v)
{
  return max(v.x, max(v.y, v.z));
}

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
in vec4 world_pos_;

// ==== Material ========

struct SpecularMaterial
{
  float exponent;
};

// ==== Lights ========

#define N_LIGHTS 4
#define N_GRADIENT_PER_LIGHT 4

#define LIGHT_TYPE_AMBIENT 0
#define LIGHT_TYPE_DIRECTIONAL 1
#define LIGHT_TYPE_POINT 2
#define LIGHT_TYPE_CUSTOM 3

#define GRADIENT_MODULATE_NORMAL 0
#define GRADIENT_MODULATE_ADD 1
#define GRADIENT_MODULATE_MULTIPLY 2

struct Gradient
{
  sampler1D curves;
  vec4 defcolor, col[4];
  float remap[2];
};

float get_curve_value(sampler1D curve, float v)
{
  return v;
//  return texture1D(curve, v); //TODO
}

vec4 get_gradient_color(Gradient g, float v)
{
  return vec4(v, v, v, 1.);
}

struct GradientLight
{
  bool use;
  float multiply_gradient_color_with_light_color;
  //float add_x_rotation, add_z_rotation; //TODO use precelcalculated values
  //float radius;
  float inside_planet;
  int modulate_type;
  Gradient light_gradient;
};
struct Light
{
  bool visible;
  int type;
  vec4 dir, pos;
  vec4 color;
  float influence_night, light_on_planet;
  float specular_factor;
  float ring_shadow;
  float cloud_shadow;
  bool just_shadows;
  Gradient shade_gradient;
  GradientLight gradient[N_GRADIENT_PER_LIGHT];
};

uniform Light light[N_LIGHTS];
uniform bool uni_no_lightning;
uniform bool uni_no_nighttexture;
float night_factor = 0.;
float night_weight = 0.;
vec4 diffuse_lightning_color = vec4(0., 0., 0., 0.);
vec4 normal;
vec4 world_pos;

void calc_diffuse_lightning()
{
  if(uni_no_lightning)
  {
    diffuse_lightning_color = vec4(1., 1., 1., 0.);
    return;
  }

  world_pos.xyz = normalize(world_pos_.xyz);
  world_pos.w = 1.;
  normal.xyz = world_pos.xyz;
  normal.w = 0.;
  float n_diff = 0.;
  
  for(int i=0; i<N_LIGHTS; ++i)
  {
    Light l = light[i];

    if(!l.visible || l.just_shadows)
      continue;

    float diff;

    switch(l.type)
    {
    case LIGHT_TYPE_DIRECTIONAL:
      diff = max(0., -dot(normal, l.dir));
      break;
    case LIGHT_TYPE_POINT:
      diff = max(0., -dot(normal, normalize(world_pos-l.pos)));
      break;
    case LIGHT_TYPE_CUSTOM:
      diff = acos(clamp(dot(normal, l.dir), -1., 1.))*INV_PI;
      break;
    case LIGHT_TYPE_AMBIENT:
    default:
      diff = 1.;
    }

    vec4 color = l.color*get_gradient_color(l.shade_gradient, diff);

    float color_intensity = max_vec3(color.xyz);

    night_factor += l.light_on_planet*l.influence_night*(1.-color_intensity);
//    night_factor += l.light_on_planet*l.influence_night*(1.-get_curve_value(night_switch_curve, color_intensity)); //TODO instead the previous line
    night_weight += l.light_on_planet*l.influence_night;

    diffuse_lightning_color += l.light_on_planet*color;
  }
}

vec4 calc_specular_lightning(SpecularMaterial m)
{
  if(uni_no_lightning)
    return vec4(0., 0., 0., 0.);
  return vec4(0., 0., 0., 0.);
}

// ============

out vec4 resulting_color;

vec4 query_surface_color();
vec4 query_cloud_color();
vec4 just_one_layer();
vec4 query_night_color();

void main()
{
  if(uni_just_one_texture_visible)
  {
    resulting_color = just_one_layer();
    return;
  }

  calc_diffuse_lightning();

  resulting_color = query_surface_color();

  resulting_color += query_night_color();

  if(uni_cloud_texture_visible)
  {
    float thickness;
    vec4 cloud_color;

    PLANET_TEXTURE_COLOR(cloud_color=, cloud);
    thickness = clamp(max(max(cloud_color.x, cloud_color.y), cloud_color.z), 0., 1.); // TODO use texture with just one component

    cloud_color = query_cloud_color();
    
    resulting_color = mix(resulting_color, cloud_color, thickness);
  }
  resulting_color.w = 1.;
}

vec4 query_surface_color()
{
  if(!uni_base_texture_visible)
    return vec4(0.5, 0.5, 0.5, 1.);

  vec4 surface_diffuse;

  PLANET_TEXTURE_COLOR(surface_diffuse=, base);

  return surface_diffuse*diffuse_lightning_color;
}

vec4 query_cloud_color()
{
  vec4 cloud_diffuse = vec4(1., 1., 1., 1.);

  return cloud_diffuse*diffuse_lightning_color;
}

vec4 query_night_color()
{
  if(uni_no_nighttexture)
    return vec4(0., 0., 0., 0.);

  vec4 night;
  PLANET_TEXTURE_COLOR(night=, night);
  return night*(night_factor/max(1., night_weight));
}

vec4 just_one_layer()
{
  if(uni_night_texture_visible)
    PLANET_TEXTURE_COLOR(return, night)
  if(uni_cloud_texture_visible)
    PLANET_TEXTURE_COLOR(return, cloud)
  if(uni_weight_texture_visible)
    PLANET_TEXTURE_COLOR(return, weight)
  PLANET_TEXTURE_COLOR(return, base)
}

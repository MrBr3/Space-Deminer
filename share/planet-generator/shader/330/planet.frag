#version 330 core

#extension GL_EXT_texture_array: enable

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

uniform sampler1DArray uni_all_curves;
uniform sampler2D uni_circle_gradient_texture;

#define PLANET_TEXTURE_COLOR(what, t) if(uni_##t##_texture_warped){what texture(uni_##t##_texture, tex_coord_warped);}else{what texture(uni_##t##_texture, tex_coord);}

in vec2 tex_coord;
in vec2 tex_coord_warped;
in vec4 world_pos_;
in vec2 rel_pos;
float distance_to_planet_center;

float get_circle_gradient_value(float x, float y)
{
  return texture(uni_circle_gradient_texture, vec2(abs(x), abs(y))).x;
}

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
  float slice_id;
  float remap_size, remap_offset;
};
struct Curve
{
  float slice_id;
};
struct ColorCurve
{
  float slice_id;
};

vec4 get_colorcurve(vec4 src_color, float slice_id)
{
  src_color.x = texture1DArray(uni_all_curves, vec2(src_color.x, slice_id)).x;
  src_color.y = texture1DArray(uni_all_curves, vec2(src_color.y, slice_id)).y;
  src_color.z = texture1DArray(uni_all_curves, vec2(src_color.z, slice_id)).z;
  src_color.w = texture1DArray(uni_all_curves, vec2(src_color.w, slice_id)).w;

  return src_color;
}

#define GET_GRADIENT_COLOR(g, x) texture1DArray(uni_all_curves, vec2((g.remap_offset+x)*g.remap_size, g.slice_id))
#define GET_CURVE_VALUE(g, v) texture1DArray(uni_all_curves, vec2(v, g.slice_id)).x
#define GET_COLOR_CURVE_VALUE(g, c) get_colorcurve(c, g.slice_id)

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
  Curve planet_shading_gradient;
  GradientLight gradient[N_GRADIENT_PER_LIGHT];
};

uniform Light light[N_LIGHTS];
uniform bool uni_no_lightning;
uniform bool uni_no_nighttexture;
uniform Gradient uni_night_gradient;
uniform bool uni_night_gradient_depends_on_diffuse;
uniform Gradient uni_cloud_gradient;
uniform ColorCurve uni_base_texture_colorcurves;
uniform ColorCurve uni_night_texture_colorcurves;
uniform Curve uni_cloud_texture_curve;
uniform ColorCurve uni_weight_texture_colorcurves;

float night_factor = 1.;
vec4 diffuse_lightning_color = vec4(0., 0., 0., 0.);
vec4 normal;
vec4 world_pos;

void calc_diffuse_lightning()
{
  if(uni_no_lightning)
  {
    diffuse_lightning_color = vec4(1., 1., 1., 0.);
    night_factor = 0.;
    return;
  }

  world_pos.xyz = normalize(world_pos_.xyz);
  world_pos.w = 1.;
  normal.xyz = world_pos.xyz;
  normal.w = 0.;
  
  for(int i=0; i<N_LIGHTS; ++i)
  {
#define l light[i]

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

    vec4 color = l.color*GET_GRADIENT_COLOR(light[i].shade_gradient, GET_CURVE_VALUE(light[i].planet_shading_gradient, diff));

    float color_intensity = max_vec3(color.xyz);

    float this_night_factor;

    if(uni_night_gradient_depends_on_diffuse)
      this_night_factor = 1.-l.light_on_planet*l.influence_night*color_intensity;
    else
      this_night_factor = acos(clamp(-dot(normal, l.dir), -1., 1.))*INV_PI;

    night_factor = clamp(this_night_factor, 0., night_factor);

    diffuse_lightning_color += l.light_on_planet*color;
#undef l
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

  distance_to_planet_center = get_circle_gradient_value(rel_pos.x, rel_pos.y);

  calc_diffuse_lightning();

  resulting_color = query_surface_color();

  resulting_color += query_night_color();

  if(uni_cloud_texture_visible)
  {
    float thickness;
    vec4 cloud_color;

    PLANET_TEXTURE_COLOR(cloud_color=, cloud);
    thickness = GET_CURVE_VALUE(uni_cloud_texture_curve, clamp(max_vec3(cloud_color.xyz), 0., 1.)); // TODO use texture with just one component

    cloud_color = query_cloud_color();
    
    resulting_color = mix(resulting_color, cloud_color, thickness*cloud_color.w);
  }
  resulting_color.w = 1.;

  resulting_color.x = distance_to_planet_center;
  resulting_color.y = resulting_color.x;
  resulting_color.z = resulting_color.x;
}

vec4 query_surface_color()
{
  vec4 surface_diffuse;

  if(uni_base_texture_visible)
  {
    PLANET_TEXTURE_COLOR(surface_diffuse=, base);
  }else
  {
    surface_diffuse = vec4(0.5, 0.5, 0.5, 1.);
  }

  return diffuse_lightning_color*GET_COLOR_CURVE_VALUE(uni_base_texture_colorcurves, surface_diffuse);
}

vec4 query_cloud_color()
{
  vec4 cloud_diffuse = vec4(1., 1., 1., 1.);

  return cloud_diffuse*GET_GRADIENT_COLOR(uni_cloud_gradient, max_vec3(diffuse_lightning_color.xyz));
}

vec4 query_night_color()
{
  if(uni_no_nighttexture)
    return vec4(0., 0., 0., 0.);

  vec4 night;
  PLANET_TEXTURE_COLOR(night=, night);
  return GET_COLOR_CURVE_VALUE(uni_night_texture_colorcurves, night)*GET_GRADIENT_COLOR(uni_night_gradient, night_factor);
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

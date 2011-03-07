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

// ==== Texture ====

uniform sampler2D uni_ring_texture;

uniform sampler1DArray uni_all_curves;

in vec2 tex_coord;
in vec4 world_pos;

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

struct Light
{
  bool visible;
  int type;
  vec4 dir, pos;
  vec4 color;
  float light_on_ring;
  float specular_factor;
  float planet_shadow;
  bool just_shadows;
  Gradient shade_gradient;
  Curve ring_shading_gradient;
};

uniform Light light[N_LIGHTS];
uniform bool uni_no_lightning;
uniform ColorCurve uni_ringtexture_colorcurves;
uniform vec4 uni_ring_normal;
uniform float uni_ring_translucency;

vec4 diffuse_lightning_color = vec4(0., 0., 0., 0.);

void calc_diffuse_lightning()
{
  if(uni_no_lightning)
  {
    diffuse_lightning_color = vec4(1., 1., 1., 1.);
    return;
  }

  float diff;
  
  for(int i=0; i<N_LIGHTS; ++i)
  {
#define l light[i]

    if(!l.visible || l.just_shadows)
      continue;

    bool flip_normal = (dot(uni_ring_normal, l.pos)<0.);
    vec4 normal;
    if(flip_normal)
      normal = uni_ring_translucency*uni_ring_normal;
    else
      normal = uni_ring_normal;

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

    vec4 color = l.color*GET_GRADIENT_COLOR(light[i].shade_gradient, GET_CURVE_VALUE(light[i].ring_shading_gradient, diff));

    diffuse_lightning_color += l.light_on_ring*color;
#undef l
  }

  diffuse_lightning_color.w = 1.;
}

vec4 calc_specular_lightning(SpecularMaterial m)
{
  if(uni_no_lightning)
    return vec4(0., 0., 0., 0.);
  return vec4(0., 0., 0., 0.);
}

// ============

out vec4 resulting_color;

void main()
{
  if(tex_coord.x>1.)
    discard;

  calc_diffuse_lightning();

  vec4 ring_color = GET_COLOR_CURVE_VALUE(uni_ringtexture_colorcurves, texture2D(uni_ring_texture, tex_coord));

  resulting_color = ring_color*diffuse_lightning_color;
}

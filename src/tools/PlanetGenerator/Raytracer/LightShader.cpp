/* Space Deminer is a small 2D Arcade Game. Your task ist to eliminate
 * intelligent Mines created in the past galactic war.
 *
 * Copyright (C) 2010 the Space Deminer Development Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include "./Raytracer.hpp"

namespace Raytracer
{
  namespace Shader
  {
    class SimpleLightShader : public Shader
    {
    public:
      ColorRGBA color;
      gfloat influence_night, light_on_planet, light_on_ring;
      gfloat specular_factor;
      gfloat ring_shadow;
      gfloat cloud_shadow;
      Vector3 dir, pos;
      //TODO lightgradient
    };
  typedef Glib::RefPtr<SimpleLightShader> SimpleLightShaderPtr;

    class AmbientLightShader : public SimpleLightShader
    {
      void shade(Param& shader_param)const
      {
        switch(shader_param.object_id)
        {
        case RING_ID:
          shader_param.result.r += color.r*light_on_ring;
          shader_param.result.g += color.g*light_on_ring;
          shader_param.result.b += color.b*light_on_ring;
          break;
        case PLANET_SURFACE_ID:
          if(mask&PLANET_NIGHT_ID)
          {
            shader_param.day_light += color.get_intensity()*influence_night;
            shader_param.n_day_lights++;
          }
        case PLANET_CLOUDS_ID:
        case PLANET_ATMOSPHERE_ID:
          shader_param.result.r += color.r*light_on_planet;
          shader_param.result.g += color.g*light_on_planet;
          shader_param.result.b += color.b*light_on_planet;
        }
      }
    };
    class DirectionalLightShader : public SimpleLightShader
    {
      void shade(Param& shader_param)const
      {
        gfloat factor = CLAMP(-shader_param.normal * dir, 0.f, 1.f);

        switch(shader_param.object_id)
        {
        case RING_ID:
          shader_param.result.r += color.r*light_on_ring*factor;
          shader_param.result.g += color.g*light_on_ring*factor;
          shader_param.result.b += color.b*light_on_ring*factor;
          break;
        case PLANET_SURFACE_ID:
          if(mask&PLANET_NIGHT_ID)
          {
            shader_param.day_light += color.get_intensity()*influence_night*factor;
            shader_param.n_day_lights++;
          }
        case PLANET_CLOUDS_ID:
        case PLANET_ATMOSPHERE_ID:
          shader_param.result.r += color.r*light_on_planet*factor;
          shader_param.result.g += color.g*light_on_planet*factor;
          shader_param.result.b += color.b*light_on_planet*factor;
        }
      }
    };
    class PointLightShader : public SimpleLightShader
    {
      void shade(Param& shader_param)const
      {
      }
    };

    SimpleLightShaderPtr lightshader_fabricator(LightLayer::LightType lt)
    {
      switch(lt)
      {
      case LightLayer::LIGHT_TYPE_AMBIENT:
        return SimpleLightShaderPtr(new AmbientLightShader());
      case LightLayer::LIGHT_TYPE_DIRECTIONAL:
        return SimpleLightShaderPtr(new DirectionalLightShader());
      case LightLayer::LIGHT_TYPE_POINT:
      default:
        return SimpleLightShaderPtr(new PointLightShader());
      }
    }
  }
  typedef Shader::SimpleLightShaderPtr SimpleLightShaderPtr;
}

using namespace ::Raytracer;

void LightLayer::create_shaders()
{
  if(!get_visible())
    return;

  {
    Shader::ObjectShadeID id = Shader::NO_ID;
    if(light_on_planet>0.f)
    {
      id |= Shader::PLANET_SURFACE_ID|Shader::PLANET_CLOUDS_ID|Shader::PLANET_ATMOSPHERE_ID;
      if(influence_night>0.f)
        id|= Shader::PLANET_NIGHT_ID;
    }
    if(light_on_ring>0.f)
      id|= Shader::RING_ID;

    if(id != Shader::NO_ID)
    {
      SimpleLightShaderPtr shader = Shader::lightshader_fabricator(LightType(light_type));
      shader->mask = id;
      shader->color.set(light_color.get_red_p()  *light_intensity,
                        light_color.get_green_p()*light_intensity,
                        light_color.get_blue_p() *light_intensity);
      shader->influence_night = influence_night;
      shader->light_on_planet = light_on_planet;
      shader->light_on_ring = light_on_ring;
      shader->specular_factor = specular_factor;
      shader->ring_shadow = ring_shadow;
      shader->cloud_shadow = cloud_shadow;
      shader->pos = position;
      shader->dir = direction;
      Shader::all_shaders.push_back(shader);
    }
  }

  // TODO Gradients
}

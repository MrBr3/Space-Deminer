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
  Planet::Planet(const Matrix44& transformation_, gfloat radius_) : sphere(transformation_, radius_)
  {
  }

  bool Planet::get_color(ColorRGBA& resulting_color, const Vector2& screen, Math::Ray ray, gfloat& distance)const
  {
    if(sphere.radius<=0.f)
      return false;

    ray.transform(sphere.inv_transformation);
    if(!sphere.intersects_local(ray, distance))
      return false;

    Vector3 p = ray.origin + ray.dir*distance;
    p.normalize();
    p *= sphere.radius;

    Vector3 normal = p;
    normal *= 1.f/sphere.radius;

    if(Manager::get_settings().get_dbg_normal())
    {
      resulting_color.set_direction(normal);
    }else
    {
      Vector2 uv;

      vec_to_uv(uv, normal);

      if(Manager::get_settings().get_dbg_uv())
      {
        resulting_color.set(uv.x, uv.y, 0.f, 1.f);
      }else
      {
        shader(resulting_color, screen, uv, normal, ray);
      }
    }

    return true;
  }

  void Planet::vec_to_uv(Vector2& uv, const Vector3& n)
  {
    gfloat inv_horizontal_radius  = 1.f/sqrt(n.x*n.x + n.y*n.y);

    uv.x  = get_angle_from_dir(n.x*inv_horizontal_radius, n.y*inv_horizontal_radius);

    uv.y  = acos(CLAMP(n.z, -1.f, 1.f))/G_PI;
  }

  void Planet::shader(ColorRGBA& color, const Vector2& screen, const Vector2& uv, const Vector3& normal, const Math::Ray& ray)
  {
    ColorRGBA base, night, clouds;

    RenderParam::get_planet_texture_color(*Texture::base_texture, base, uv.x, uv.y);
    RenderParam::get_planet_texture_color(*Texture::night_texture, night, uv.x, uv.y);
    base.a = 1.f;
    night.a = 1.f;

    if(Manager::get_settings().get_dbg_unlit_base_texture())
    {
      color = base;
      return;
    }

    RenderParam::get_planet_texture_color(*Texture::cloud_layer, clouds, uv.x, uv.y);

    gfloat cloud_alpha  = clouds.a*(clouds.r+clouds.g+clouds.b)/3.f;

    Shader::Param sp(Shader::PLANET_SURFACE_ID);

    sp.diffuse = base; // mehrere Materialien
    sp.specular.set(0.f, 0.f, 0.f); // mehrere Materialien
    sp.specular_width = 0.f; // mehrere Materialien
    sp.specular_strength = 0.f; // mehrere Materialien
    sp.view = ray.dir;
    sp.normal = normal; // TODO: NormalMaps
    sp.geometry_normal = normal;
    sp.screen_pos = screen;

    shade(sp);

    base.r *= sp.result.r;
    base.g *= sp.result.b;
    base.b *= sp.result.g;

    if(sp.n_day_lights>0)
    {
      sp.day_light = 1.f-sp.day_light/sp.n_day_lights;
      //TODO stärke der Nacht letztendlich mit extra Kurve bestimmen
      base.r += night.r * sp.day_light;
      base.g += night.g * sp.day_light;
      base.b += night.b * sp.day_light;
    }

    // TODO: Atmosphäre

    // TODO: Wolken auch durch den shader jagen
    color.r = base.r*(1.f-cloud_alpha)+cloud_alpha;
    color.g = base.g*(1.f-cloud_alpha)+cloud_alpha;
    color.b = base.b*(1.f-cloud_alpha)+cloud_alpha;
    color.a = 1.f;
  }
}

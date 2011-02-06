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

  bool Planet::get_color(ColorRGBA& resulting_color, Math::Ray ray, gfloat& distance)const
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
        //if(Manager::get_settings().get_dbg_unlit_base_texture())
        shader(resulting_color, uv, normal);
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

  void Planet::shader(ColorRGBA& color, const Vector2& uv, const Vector3& normal)
  {
    Texture::base_texture->get_color(color, uv.x, uv.y, Texture::WRAP_REPEAT, Texture::WRAP_CLAMPED);
  }
}
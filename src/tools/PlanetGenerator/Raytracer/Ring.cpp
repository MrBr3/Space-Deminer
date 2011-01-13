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
  Ring::Ring(const Matrix44& planet_transformation, const Matrix44& ring_transformation) : plane(Vector3(0.f, 0.f, 1.f), 0.f), transformation(ring_transformation), inv_transformation(ring_transformation)
  {
    Matrix44 inv_planet_transformation  = planet_transformation;
    inv_planet_transformation.invert();
    inv_transformation.invert();

    normal  = inv_planet_transformation * ring_transformation * Vector4(0.f, 0.f, 1.f, 0.f);
    normal.normalize();
    inv_normal  = normal;
    inv_normal  *= -1.f;

    const RingLayer& rl = *RingLayer::get_singleton();
    g_assert(&rl);

    outer_radius  = rl.get_outer_radius();
    inner_radius  = rl.get_inner_radius();
    inv_outer_minus_inner_radius  = 1.f/(outer_radius-inner_radius);
    outer_radius_pow_2  = outer_radius*outer_radius;
    inner_radius_pow_2  = inner_radius*inner_radius;
    visible = rl.get_visible();
  }

  bool Ring::get_color(ColorRGBA& color, Math::Ray ray, gfloat& distance)const
  {
    if(!visible)
      return false;

    ray.transform(inv_transformation);
    if(!plane.check_ray(ray, distance))
      return false;

    Vector3 p = ray.origin + ray.dir*distance;

    if(p.get_square_length()>outer_radius_pow_2 || p.get_square_length()<inner_radius_pow_2)
      return false;

    const Vector3& n = ray.origin.z>0.f ? normal : inv_normal;

    gfloat distance_from_center = p.get_length();

    if(Manager::get_settings().get_dbg_normal())
    {
      color.set_direction(n);
    }else
    {
      Vector2 uv(1.f-(distance_from_center-inner_radius)*inv_outer_minus_inner_radius, 0.5f);

      if(Manager::get_settings().get_dbg_uv())
      {
        color.set(uv.x, uv.y, 0.f, 1.f);
      }else
      {
        //if(Manager::get_settings().get_dbg_unlit_base_texture())
        shader(color, uv, n);
      }
    }

    return true;
  }

  void Ring::shader(ColorRGBA& color, const Vector2& uv, const Vector3& normal)
  {
    color.set(1.f, 0.5f, 0.f, 1.f);
  }
}
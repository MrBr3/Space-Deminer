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
  Geometry::Geometry(const Matrix44& transformation_) : transformation(transformation_), inv_transformation(transformation_)
  {
    inv_transformation.invert();
  }

  bool Geometry::get_color(ColorRGBA& color, Ray ray)const
  {
    color.r = 0.f;
    color.g = 0.f;
    color.b = 0.f;
    color.a = 0.f;

    ray.transform(inv_transformation);

    return v_get_color(color, ray);
  }

  //-------------

  Sphere::Sphere(const Matrix44& transformation_, gfloat radius_) : Geometry(transformation_), radius(radius_)
  {
  }

  bool Sphere::v_get_color(ColorRGBA& resulting_color, const Ray& ray)const
  {
    gfloat t1, t2;

    if(!solve_quadric_formula(t1, t2,
                              square(ray.dir.x) + square(ray.dir.y) + square(ray.dir.z),
                              2.f*(ray.dir.x*ray.origin.x + ray.dir.y*ray.origin.y + ray.dir.z*ray.origin.z),
                              square(ray.origin.x) + square(ray.origin.y) + square(ray.origin.z) - square(radius)))
      return false;

    if(t1<0.f && t2<0.f)
      return false;

    if(t1<0.f)
      t1 = t2;

    if(t2<0.f)
      t2 = t1;

    t1  = MIN(t1, t2);

    Vector3 p = ray.origin + ray.dir*t1;
    Vector3 normal = p;
    normal.normalize();

    resulting_color.set(0.5f*normal.x+0.5f, 0.5f*normal.y+0.5f, 0.5f*normal.z+0.5f, 1.f);

    return true;
  }

  void Sphere::vec_to_uv(Vector2& uv, const Vector3& p)
  {/*
    if(p.x>p.y)
      uv.x  = acos(p.x)/G_PI;
    else
      uv.x  = acos(p.y)/G_PI;

    gfloat d  = sqrt(p.x*p.x+p.y*p.y);

    if(p.z>d)
      uv.y  = sin()*/
  }

  //-------------

  void get_planet_color(ColorRGBA& color, const Vector2& uv, const Vector3& normal)
  {
    color.r = uv.x;
    color.g = uv.y;
    color.b = 0.f;
    color.a = 1.f;
  }
}
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

#ifndef _SPACE_DEMINER_BASE_MATH_SPHERE_H_
#define _SPACE_DEMINER_BASE_MATH_SPHERE_H_

class Sphere : public Geometry
{
public:
  gfloat radius;

  bool intersects_local(const Ray& ray, gfloat& first_intersection)const
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
      first_intersection = t2;
    else if(t2<0.f)
      first_intersection = t1;
    else
      first_intersection  = MIN(t1, t2);
    return true;
  }
  
  /** \brief Calculates the position of and a point in the circle, covering the sphere.
   * 
   * \param circle_center the world position of the center of the circle (previous value is ignored)
   * \param circle_point a point on the circle (previous value is ignored)
   * \param view_maxtrix the view matrix of the camera
   */ 
  void screen_circle_world(Vector3& circle_center, Vector3& circle_point, const Matrix44& view_maxtrix);
  
  /** \brief Calculates the position and radius of the circle, covering the sphere.
   * 
   * \param circle_center the screen position in pixels of the center of the circle (previous value is ignored)
   * \param circle_radius the circle's radius on the screen (each component for each axis. 2 means full screen) (previous value is ignored)
   * \param projection_maxtrix the projection matrix of the camera
   * \param view_maxtrix the view matrix of the camera
   * \param screen_width
   * \param screen_height
   */ 
  void screen_circle_pixelscreen(Vector2& circle_center, gfloat& circle_radius, const Matrix44& projection_maxtrix, const Matrix44& view_maxtrix, gfloat screen_width, gfloat screen_height);

  Sphere()
  {
    radius = 1.f;
  }
  Sphere(const Matrix44& transformation_, gfloat radius_) : Geometry(transformation_), radius(radius_)
  {
  }
};

inline Overlap Plane::check_sphere(const Sphere& sphere, gfloat epsilon)const
{
  Vector3 sphere_pos  = sphere.get_position();

  Overlap o = check_point(sphere_pos, sphere.radius+epsilon);

  if(o==INSIDE)
    return OVERLAPPING;

  return o;
}

#endif
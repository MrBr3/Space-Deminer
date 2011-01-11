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

#ifndef _SPACE_DEMINER_BASE_MATH_GEOMETRY_H_
#define _SPACE_DEMINER_BASE_MATH_GEOMETRY_H_

class Geometry
{
public:
  Matrix44 transformation;
  Matrix44 inv_transformation;

  /** \brief Gets the nearest intersection of a ray in global local space
   * */
  bool intersects_global(Ray ray, gfloat& first_intersection)const
  {
    ray.transform(inv_transformation);
    return intersects_local(ray, first_intersection);
  }

  /** \brief Gets the nearest intersection of a ray in the geometries local space
   * */
  virtual bool intersects_local(const Ray& ray, gfloat& first_intersection)const=0;

  Geometry()
  {
  }

  Geometry(const Matrix44& transformation_) : transformation(transformation_), inv_transformation(transformation_)
  {
    inv_transformation.invert();
  }
};

#endif
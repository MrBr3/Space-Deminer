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

namespace Raytracer
{
  class Ray;
  class RenderParam;
  class Geometry
  {
  protected:
    /** \brief Gets the color by a ray
     *
     * \param color the resulting color, if not hit, this color won't be changed
     * \param ray a ray in modelspace
     * \param render_param
     *
     * \return true if hit, otherwise false
     * */
    virtual bool v_get_color(ColorRGBA& color, const Ray& ray)const = 0;

  public:
    const Matrix44& transformation;
    Matrix44 inv_transformation;

    /** \brief Gets the color by a ray
     *
     * \param color the resulting color, if not hit, this color fully transparent
     * \param ray a ray in worldspace
     * \param render_param
     *
     * \return true if hit, otherwise false
     * */
    bool get_color(ColorRGBA& color, Ray ray)const;

    Geometry(const Matrix44& transformation_);
  };

  class Sphere : public Geometry
  {
  protected:
    bool v_get_color(ColorRGBA& color, const Ray& ray)const;

    static void vec_to_uv(Vector2& uv, const Vector3& p);

  public:
    gfloat radius;

    Sphere(const Matrix44& transformation_, gfloat radius_);
  };

  void get_planet_color(ColorRGBA& color, const Vector2& uv, const Vector3& normal, const RenderParam& render_param);
}
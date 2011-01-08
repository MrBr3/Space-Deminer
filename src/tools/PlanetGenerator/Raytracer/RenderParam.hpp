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
  class RenderParam : public Refable
  {
    //gfloat _sphere_center_x, _sphere_center_y;
    //gfloat _inv_sphere_radius_y_dir;
  public:
    Sphere planet;
    const Matrix44& view_matrix;
    const Matrix44& projection_matrix;
    Matrix44 inv_view_matrix;
    Matrix44 inv_projection_matrix;

    const int img_width, img_height;
    gfloat inv_img_width, inv_img_height;
    gfloat aspect;
    gfloat inv_aspect;
    int rays_per_pixel;

    /* TODO uncomment * \brief Gets the relative
     * */
    /*gfloat get_distance_to_planet_center(gfloat rel_x, gfloat rel_y)const throw()
    {
      return sqrt(square((_sphere_center_x-rel_x)*aspect)+square(_sphere_center_y-rel_y))*_inv_sphere_radius_y_dir;
    }*/

  public:
    static Glib::RefPtr<RenderParam> create(int img_width, int img_height);
    static Glib::RefPtr<RenderParam> create(const Sphere& planet_, const Matrix44& view_matrix_, const Matrix44& projection_matrix_,
                                            int img_width_, int img_height_, int antialiasing_)
    {
      return Glib::RefPtr<RenderParam>(new RenderParam(planet_, view_matrix_, projection_matrix_, img_width_, img_height_, antialiasing_));
    }

private:
    RenderParam(const Sphere& planet_,  const Matrix44& view_matrix_, const Matrix44& projection_matrix_,
                int img_width_, int img_height_, int antialiasing) :
                          planet(planet_), view_matrix( view_matrix_), projection_matrix(projection_matrix_),
                          inv_view_matrix(view_matrix_), inv_projection_matrix(projection_matrix_),
                          img_width(img_width_), img_height(img_height_)
    {
      g_assert(img_width>0);
      g_assert(img_height>0);
      g_assert(antialiasing>=0 && antialiasing<4);

      //_sphere_center_x  = 0.5f;
      //_sphere_center_y  = 0.5f;
      //_inv_sphere_radius_y_dir

      inv_view_matrix.invert();
      inv_projection_matrix.invert();
      inv_img_width  = 1.f/gfloat(img_width);
      inv_img_height  = 1.f/gfloat(img_height);
      aspect  = gfloat(img_width)/gfloat(img_height);
      inv_aspect  = 1.f/aspect;

      rays_per_pixel  = 1<<antialiasing;
      g_assert(rays_per_pixel==1 || rays_per_pixel==2 || rays_per_pixel==4 || rays_per_pixel==8);
    }
  };
}
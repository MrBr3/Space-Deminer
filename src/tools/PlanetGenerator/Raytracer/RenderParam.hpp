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
    Planet planet;
    Ring ring;
    Math::Sphere bounding_sphere; // bounding sphere for the planet including it's atmosphere
    Vector3 bounding_ngon[8]; // bounding ngon for the planets ring
    const Matrix44& view_matrix;
    const Matrix44 projection_matrix;
    Matrix44 inv_view_matrix;
    Matrix44 inv_projection_matrix;

    bool is_something_visible_within(int x, int y, int w, int h)const;

    const int img_width, img_height;
    gfloat inv_img_width, inv_img_height;
    gfloat aspect;
    gfloat inv_aspect;
    int rays_per_pixel;

    bool culling;
    gfloat culling_epsilon;

    void get_ray_dir(Vector3& dir, gfloat x, gfloat y)const;
    void get_camera_pos(Vector3& dir)const;

    /* TODO uncomment * \brief Gets the relative
     * */
    /*gfloat get_distance_to_planet_center(gfloat rel_x, gfloat rel_y)const throw()
    {
      return sqrt(square((_sphere_center_x-rel_x)*aspect)+square(_sphere_center_y-rel_y))*_inv_sphere_radius_y_dir;
    }*/

  public:
    static Glib::RefPtr<RenderParam> create(int img_width, int img_height);
    static Glib::RefPtr<RenderParam> create(const Matrix44& ring_matrix, const Matrix44& planet_matrix, const Matrix44& view_matrix_, const Matrix44& projection_matrix_,
                                            int img_width_, int img_height_, int antialiasing_)
    {
      return Glib::RefPtr<RenderParam>(new RenderParam(ring_matrix, planet_matrix, view_matrix_, projection_matrix_, img_width_, img_height_, antialiasing_));
    }

private:
    RenderParam(const Matrix44& ring_matrix, const Matrix44& planet_matrix,  const Matrix44& view_matrix_, const Matrix44& projection_matrix_,
                int img_width_, int img_height_, int antialiasing);
  };
}
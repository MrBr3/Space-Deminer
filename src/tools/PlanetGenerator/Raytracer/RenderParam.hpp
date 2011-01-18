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
    struct CullingCircle
    {
      gfloat r;
      gfloat x, y;

      gfloat x_dist(gfloat x)const
      {
        return fabs(x-this->x);
      }

      gfloat y_dist(gfloat y)const
      {
        return fabs(y-this->y);
      }

      gfloat square_dist(gfloat x, gfloat y)const
      {
        return x_dist(x)*x_dist(x)+y_dist(y)*y_dist(y);
      }

      operator Vector2()const
      {
        return Vector2(x, y);
      }

      operator Vector3()const
      {
        return Vector2(x, y);
      }

      void set(const Vector2& pos, gfloat radius=INV_SQRT_2+1.e-4f)
      {
        r = MAX(1.f, radius);
        x = pos.x;
        y = pos.y;
      }

      bool is_within_tile(int tile_x, int tile_y, int tile_w, int tile_h, gfloat epsilon)const
      {
        bool intersect_h  = x>=tile_x && x<tile_x+tile_w;
        bool intersect_v = y>=tile_y && y<tile_y+tile_h;

        if(intersect_h && intersect_v)
          return true;

        if(intersect_v)
          return x_dist(tile_x) <= r+epsilon || x_dist(tile_x+tile_w) <= r+epsilon;

        if(intersect_h)
          return y_dist(tile_y) <= r+epsilon || y_dist(tile_y+tile_h) <= r+epsilon;

        gfloat rr = r*r+epsilon;

        return square_dist(tile_x, tile_y)<=rr ||
               square_dist(tile_x+tile_w, tile_y)<=rr ||
               square_dist(tile_x, tile_y+tile_h)<=rr ||
               square_dist(tile_x+tile_w, tile_y+tile_h)<=rr;
      }
    };

    Planet planet;
    Ring ring;
    CullingCircle bounding_sphere;
    CullingCircle bounding_ngon[8]; // bounding ngon for the planets ring
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
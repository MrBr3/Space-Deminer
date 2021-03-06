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
    
    static RenderParam* _singleton;
  public:
    static RenderParam* singleton(){return _singleton;}
    static RenderParam* singletonA(){g_assert(_singleton);return _singleton;}
  
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

      bool is_within_tile(int tile_x, int tile_y, int tile_w, int tile_h, gfloat e)const
      {
        bool intersect_h  = x>=tile_x && x<tile_x+tile_w;
        bool intersect_v = y>=tile_y && y<tile_y+tile_h;

        if(intersect_h && intersect_v)
          return true;

        if(intersect_v)
          return x_dist(tile_x) <= r || x_dist(tile_x+tile_w) <= r;

        if(intersect_h)
          return y_dist(tile_y) <= r || y_dist(tile_y+tile_h) <= r;

        gfloat rr = r*r+e;

        return square_dist(tile_x, tile_y)<=rr ||
               square_dist(tile_x+tile_w, tile_y)<=rr ||
               square_dist(tile_x, tile_y+tile_h)<=rr ||
               square_dist(tile_x+tile_w, tile_y+tile_h)<=rr;
      }
    };

    Planet planet;
    Ring ring;
    CullingCircle bounding_sphere;
    Vector2 bounding_ngon[8]; // bounding ngon for the planets ring
    const Matrix44& view_matrix;
    const Matrix44 projection_matrix;
    Matrix44 inv_view_matrix;
    Matrix44 inv_projection_matrix;
    
    bool filter_ring_textures;
    gfloat texture_filter_coefficients[max_texture_filtering_samples];
    Vector2 texture_filter_coordinates[max_texture_filtering_samples];
    gsize n_texture_filter_samples;
    gfloat texture_filter_radius;
    
    static Planet& get_planet()
    {
      return singletonA()->planet;
    }
    
    static Ring& get_ring()
    {
      return singletonA()->ring;
    }
    
    void get_filtered_texture_color(const Texture& tex, ColorRGBA& color, gfloat u, gfloat v, gfloat blur_amount, Texture::WrapMode u_mode, Texture::WrapMode v_mode);
    
    static void get_planet_texture_color(const Texture& tex, ColorRGBA& color, gfloat u, gfloat v, Texture::WrapMode u_mode=Texture::WRAP_REPEAT, Texture::WrapMode v_mode=Texture::WRAP_CLAMPED)
    {
      tex.get_color(color, u, v, u_mode, v_mode);
    }
    
    static void get_ring_texture_color(const Texture& tex, ColorRGBA& color, gfloat u, gfloat v, gfloat blur_amount, Texture::WrapMode u_mode=Texture::WRAP_TRANSPARENT, Texture::WrapMode v_mode=Texture::WRAP_CLAMPED)
    {
      if(singletonA()->filter_ring_textures)
        singletonA()->get_filtered_texture_color(tex, color, u, v, blur_amount, u_mode, v_mode);
      else
        tex.get_color(color, u, v, u_mode, v_mode);
    }

    static bool is_something_visible_within(int x, int y, int w, int h);

    const int img_width, img_height;
    gfloat inv_img_width, inv_img_height;
    gfloat aspect;
    gfloat inv_aspect;
    int rays_per_pixel;

    bool culling;
    gfloat culling_epsilon;

    static void get_ray_dir(Vector3& dir, gfloat x, gfloat y);
    static void get_camera_pos(Vector3& dir);

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
    ~RenderParam()throw();
  };
}
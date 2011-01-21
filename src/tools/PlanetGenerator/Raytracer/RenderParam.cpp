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

#include "./../MainWindow.hpp"

namespace Raytracer
{
  RenderParam::RenderParam(const Matrix44& ring_matrix, const Matrix44& planet_matrix,  const Matrix44& view_matrix_, const Matrix44& projection_matrix_,
                           int img_width_, int img_height_, int antialiasing) :
                           planet(planet_matrix, 1.f), ring(planet_matrix, ring_matrix),
                           view_matrix( view_matrix_), projection_matrix(projection_matrix_),
                           inv_view_matrix(view_matrix_), inv_projection_matrix(projection_matrix_),
                           img_width(img_width_), img_height(img_height_)
  {
    g_assert(img_width>0);
    g_assert(img_height>0);
    g_assert(antialiasing>=0 && antialiasing<4);

    culling = Manager::get_settings().get_culling();
    culling_epsilon = Manager::get_settings().get_culling_epsilon();

    inv_view_matrix.invert();
    inv_projection_matrix.invert();
    inv_img_width  = 1.f/gfloat(img_width);
    inv_img_height  = 1.f/gfloat(img_height);
    aspect  = gfloat(img_width)/gfloat(img_height);
    inv_aspect  = 1.f/aspect;

    rays_per_pixel  = 1<<antialiasing;
    g_assert(rays_per_pixel==1 || rays_per_pixel==2 || rays_per_pixel==4 || rays_per_pixel==8);

    if(culling)
    {
      Vector3 sphere_outest_point;
      Vector3 sphere_middle;

      // Get the seeming radius of the planet
      {
        Vector3 frustrum_top_left;
        Vector3 frustrum_top_middle;
        Vector3 frustrum_top_right;

        get_ray_dir(frustrum_top_left, -1.f, 1.f);
        get_ray_dir(frustrum_top_middle, 0.f, 1.f);
        get_ray_dir(frustrum_top_right, 1.f, 1.f);

        sphere_outest_point = cross(frustrum_top_middle, frustrum_top_left-frustrum_top_right);
        sphere_outest_point.normalize();

        sphere_outest_point *= 1.f; // TODO multiply in order to see also the atmosphere
      }

      sphere_outest_point = projection_matrix * view_matrix * sphere_outest_point;
      sphere_middle = projection_matrix * view_matrix * Vector3(0.f, 0.f, 0.f);

      sphere_outest_point.x = (sphere_outest_point.x+1.f) * 0.5f * img_width;
      sphere_outest_point.y =-(sphere_outest_point.y-1.f) * 0.5f * img_height;
      sphere_middle.x = (sphere_middle.x+1.f) * 0.5f * img_width;
      sphere_middle.y =-(sphere_middle.y-1.f) * 0.5f * img_height;

      bounding_sphere.r = ceil((sphere_outest_point-sphere_middle).get_length());
      bounding_sphere.x = sphere_middle.x;
      bounding_sphere.y = sphere_middle.y;

      if(ring.visible)
      {
        Vector3 tmp[8];

        gfloat o  = 2.f*INV_SQRT_2-1.f+1.e-4f;
        tmp[0].set( 1.f,   o, 0.f);
        tmp[1].set(   o, 1.f, 0.f);
        tmp[2].set(  -o, 1.f, 0.f);
        tmp[3].set(-1.f,   o, 0.f);
        tmp[4].set(-1.f,  -o, 0.f);
        tmp[5].set(  -o,-1.f, 0.f);
        tmp[6].set(   o,-1.f, 0.f);
        tmp[7].set( 1.f,  -o, 0.f);

        for(int i=0; i<8; ++i)
        {
          tmp[i]  = projection_matrix * view_matrix * ring_matrix * (tmp[i]*ring.outer_radius);
          bounding_ngon[i].x  = (tmp[i].x+1.f)*0.5f;
          bounding_ngon[i].y  = (tmp[i].y-1.f)*-0.5f;
          bounding_ngon[i].x *= img_width;
          bounding_ngon[i].y *= img_height;
        }
      }
    }
  }

  Glib::RefPtr<RenderParam> RenderParam::create(int img_width, int img_height)
  {
    const Settings& settings = Manager::get_settings();
    const View3D& view3d = main_window->get_view_3d();

    g_assert(&settings);
    g_assert(&view3d);

    Matrix44 m;
    view3d.calc_projection_matrix(m, gfloat(img_width)/gfloat(img_height));

    return RenderParam::create(view3d.ring_model_matrix, view3d.planet_model_matrix, view3d.view_matrix, m, img_width, img_height, settings.get_antialiasing());
  }

  void RenderParam::get_ray_dir(Vector3& dir, gfloat x, gfloat y)const
  {
    dir = inv_projection_matrix * Vector3(x, y, 0.f);
    dir = inv_view_matrix * Vector4(dir, 0.f);
    dir.normalize();
  }

  void RenderParam::get_camera_pos(Vector3& pos)const
  {
    pos = inv_view_matrix * Vector3(0.f, 0.f, 0.f);
  }

  bool RenderParam::is_something_visible_within(int x, int y, int w, int h)const
  {
    if(!culling)
      return true;

    x-=culling_epsilon;
    y-=culling_epsilon;
    w+=culling_epsilon*2;
    h+=culling_epsilon*2;

    bool visible_planet;
    bool visible_ring = false;

    visible_planet = bounding_sphere.is_within_tile(x, y, w, h);

    if(ring.visible && !visible_planet)
    {
      for(int i=0; !visible_ring && i<8; ++i)
      {
        const Vector2& curr = bounding_ngon[i];

        visible_ring |= (curr.x>=x) && (curr.y>=y) && (curr.x<=x+w) && (curr.y<=y+h);
      }

      Vector2 a(x,y), b(x, y+h), c(x+w, y+h), d(x+w, y);

      if(!visible_ring)
      {
        visible_ring |= within_ngon(a, bounding_ngon, 8);
        visible_ring |= within_ngon(b, bounding_ngon, 8);
        visible_ring |= within_ngon(c, bounding_ngon, 8);
        visible_ring |= within_ngon(d, bounding_ngon, 8);
      }

      if(!visible_ring)
      {
        for(int i=0; !visible_ring && i<8; ++i)
        {
          const Vector2& curr = bounding_ngon[i];
          const Vector2& next = bounding_ngon[(i+1)%8];

          visible_ring |= line_cuts_hline(curr, next, a, w, false);
          visible_ring |= line_cuts_hline(curr, next, b, w, false);
          visible_ring |= line_cuts_vline(curr, next, a, h);
          visible_ring |= line_cuts_vline(curr, next, d, h);
        }
      }
    }

    return visible_planet || visible_ring;
  }
}
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
                           planet(planet_matrix, 1.f), ring(planet_matrix, ring_matrix), bounding_sphere(Matrix44::identity, 1.f),
                           view_matrix( view_matrix_), projection_matrix(projection_matrix_),
                           inv_view_matrix(view_matrix_), inv_projection_matrix(projection_matrix_),
                           img_width(img_width_), img_height(img_height_)
  {
    g_assert(img_width>0);
    g_assert(img_height>0);
    g_assert(antialiasing>=0 && antialiasing<4);

    if(ring.visible)
    {
      bounding_ngon[0].set(1.f, 0.f, 0.f);
      bounding_ngon[1].set( INV_SQRT_2, INV_SQRT_2, 0.f);
      bounding_ngon[2].set(0.f, 1.f, 0.f);
      bounding_ngon[3].set(-INV_SQRT_2, INV_SQRT_2, 0.f);
      bounding_ngon[4].set(-1.f, 0.f, 0.f);
      bounding_ngon[5].set(-INV_SQRT_2,-INV_SQRT_2, 0.f);
      bounding_ngon[6].set(0.f,-1.f, 0.f);
      bounding_ngon[7].set( INV_SQRT_2,-INV_SQRT_2, 0.f);

      for(int i=0; i<8; ++i)
        bounding_ngon[i]  *= ring.outer_radius;
    }

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
    Plane frustrum[5];
    Vector3 camera_pos;
    Vector3 ray_dir_e, ray_dir_n, ray_dir_w, ray_dir_s;
    Vector3 y_axis, x_axis, camera_dir;

    gfloat rel_w = (x-1.f)*inv_img_width*2.f-1.f;
    gfloat rel_n =-(y-1.f)*inv_img_height*2.f+1.f;
    gfloat rel_e = (x+1.f+w)*inv_img_width*2.f-1.f;
    gfloat rel_s =+(y+1.f+h)*inv_img_height*2.f+1.f;

    get_camera_pos(camera_pos);
    get_ray_dir(ray_dir_w, rel_w, (rel_n+rel_s)*0.5f);
    get_ray_dir(ray_dir_e, rel_e, (rel_n+rel_s)*0.5f);
    get_ray_dir(ray_dir_n, (rel_w+rel_e)*0.5f, rel_n);
    get_ray_dir(ray_dir_s, (rel_w+rel_e)*0.5f, rel_s);
    get_ray_dir(camera_dir, (rel_w+rel_e)*0.5f, (rel_n+rel_s)*0.5f);
    x_axis = ray_dir_e-ray_dir_w;
    y_axis = ray_dir_n-ray_dir_s;

    frustrum[0].set( y_axis, ray_dir_w, camera_pos);
    frustrum[1].set(-y_axis, ray_dir_e, camera_pos);
    frustrum[2].set(-x_axis, ray_dir_n, camera_pos);
    frustrum[3].set( x_axis, ray_dir_s, camera_pos);
    frustrum[4].set(-camera_dir, camera_pos);

    bool visible_planet = true;
    bool visible_ring = ring.visible;

    for(int i=0; i<5 && (visible_planet||visible_ring); ++i)
    {
      if(visible_planet)
      {
        visible_planet &= frustrum[i].check_sphere(bounding_sphere) != Math::BACKSIDE;
      }
      if(visible_ring)
      {
        visible_ring  = false;
        //visible_ring &= frustrum[i].check_sphere()!=Math::BACKSIDE;
      }
    }

    return visible_planet || visible_ring;
  }
}
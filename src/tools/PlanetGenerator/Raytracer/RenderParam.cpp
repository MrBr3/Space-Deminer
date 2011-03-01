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
  RenderParam* RenderParam::_singleton = nullptr;

  RenderParam::RenderParam(const Matrix44& ring_matrix, const Matrix44& planet_matrix,  const Matrix44& view_matrix_, const Matrix44& projection_matrix_,
                           int img_width_, int img_height_, int antialiasing) :
                           planet(planet_matrix, 1.f), ring(ring_matrix),
                           view_matrix( view_matrix_), projection_matrix(projection_matrix_),
                           inv_view_matrix(view_matrix_), inv_projection_matrix(projection_matrix_),
                           img_width(img_width_), img_height(img_height_)
  {
    g_assert(!_singleton);
    _singleton  = this;
    g_assert(img_width>0);
    g_assert(img_height>0);
    g_assert(antialiasing>=0 && antialiasing<9);

    // Prepare Texture Filtering
    {
      n_texture_filter_samples  = Manager::get_settings().get_texture_filtering_n_samples();
      filter_ring_textures  = Manager::get_settings().get_texture_filtering_in_ring() && n_texture_filter_samples>0;
      texture_filter_radius  = Manager::get_settings().get_texture_filtering_radius();

      if(!filter_ring_textures)
        n_texture_filter_samples = 0;

      g_assert(n_texture_filter_samples>=0 && n_texture_filter_samples<=max_texture_filtering_samples);

      if(n_texture_filter_samples>0)
      {
        gdouble inv_2pi = 1.f/(PI2);
        gdouble tmp;
        gdouble f=0.;

        for(gsize i=0; i<n_texture_filter_samples; ++i)
        {
          if(i==0)
            texture_filter_coordinates[i].set(0.f, 0.f);
          else
            texture_filter_coordinates[i].set(2.f*frand()-1.f, 2.f*frand()-1.f);

          gdouble x = texture_filter_coordinates[i].x*3.;
          gdouble y = texture_filter_coordinates[i].y*3.;

          texture_filter_coefficients[i] = tmp = inv_2pi * exp(-(x*x+y*y)*0.5);
          f += tmp;
        }
        g_assert(f>0.);
        f = 1./f;

        for(gsize i=0; i<n_texture_filter_samples; ++i)
        {
          texture_filter_coefficients[i] *=f;
        }
      }
    }

    culling = Manager::get_settings().get_culling();
    culling_epsilon = Manager::get_settings().get_culling_epsilon();

    inv_view_matrix.invert();
    inv_projection_matrix.invert();
    inv_img_width  = 1.f/gfloat(img_width);
    inv_img_height  = 1.f/gfloat(img_height);
    aspect  = gfloat(img_width)/gfloat(img_height);
    inv_aspect  = 1.f/aspect;

    rays_per_pixel  = 1<<antialiasing;
    g_assert(rays_per_pixel==1 || rays_per_pixel==2 || rays_per_pixel==4 || rays_per_pixel==8 || rays_per_pixel==16 || rays_per_pixel==32 || rays_per_pixel==64 || rays_per_pixel==128 || rays_per_pixel==256);

    if(culling)
    {
      Vector3 sphere_outest_point;
      Vector3 sphere_middle;

      // Get the seeming radius of the planet
      {
        Vector2 sphere_middle;

        planet.sphere.screen_circle_pixelscreen(sphere_middle, bounding_sphere.r, projection_matrix, view_matrix, img_width, img_height);
        bounding_sphere.x = sphere_middle.x;
        bounding_sphere.y = sphere_middle.y;
      }

      //----

      Shader::all_shaders.clear();

      for(gsize i=0; i<N_LIGHT_LAYERS; ++i)
      {
        LightLayer::get_singleton(i)->create_shaders();
      }

      //----

      if(ring.visible)
      {
        Vector3 camera_origin(DONT_INIT);
        Vector3 camera_dir(DONT_INIT);
        Plane camera_plane(DONT_INIT);

        RenderParam::get_ray_dir(camera_dir, 0.f, 0.f);
        RenderParam::get_camera_pos(camera_origin);

        camera_dir.normalize();
        camera_plane.set(camera_dir, camera_origin);

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
          tmp[i]  = ring_matrix * (tmp[i]*ring.outer_radius);
          if(camera_plane.check_point(tmp[i])==Math::BACKSIDE)
          {
            culling=false;
          }
          tmp[i]  = projection_matrix * view_matrix * tmp[i];
          bounding_ngon[i].x  = (tmp[i].x+1.f)*0.5f;
          bounding_ngon[i].y  = (tmp[i].y-1.f)*-0.5f;
          bounding_ngon[i].x *= img_width;
          bounding_ngon[i].y *= img_height;
        }

        if(n_behind>=8)
          ring.visible = false;
      }
    }
  }

  RenderParam::~RenderParam()throw()
  {
    g_assert(_singleton);
    _singleton = nullptr;
  }

  void RenderParam::get_filtered_texture_color(const Texture& tex, ColorRGBA& color, gfloat u, gfloat v, gfloat blur_amount, Texture::WrapMode u_mode, Texture::WrapMode v_mode)
  {
    /*blur_amount = 1.f-blur_amount;
    blur_amount = 1.f-blur_amount*blur_amount;
    blur_amount = 1.f;*/
    gfloat biger_len = 0.5f/MAX(1.f, MAX(tex.get_height(), tex.get_width()));

    gfloat r = biger_len+CLAMP(blur_amount, 0.f, 1.f)*MAX(texture_filter_radius-biger_len, 0.f)*0.01f; // TODO

    color.set(0.f, 0.f, 0.f, 0.f);

    ColorRGBA tmp;

    for(gsize i=0; i<n_texture_filter_samples; ++i)
    {
      gfloat coeff = texture_filter_coefficients[i];
      Vector2 coord = texture_filter_coordinates[i];

      tex.get_color(tmp, u+coord.x*r, v+coord.y*r, u_mode, v_mode);

      color.a += tmp.a*coeff;
      color.r += tmp.r*tmp.a*coeff;
      color.g += tmp.g*tmp.a*coeff;
      color.b += tmp.b*tmp.a*coeff;
    }

    if(color.a==0.f)
    {
      color.set(0.f, 0.f, 0.f, 0.f);
      return;
    }

    gfloat inv_a  = 1.f/color.a;
    color.r *= inv_a;
    color.g *= inv_a;
    color.b *= inv_a;
  }

  Glib::RefPtr<RenderParam> RenderParam::create(int img_width, int img_height)
  {
    const Settings& settings = Manager::get_settings();
    const View3D& view3d = main_window->get_view_3dA();

    g_assert(&settings);
    g_assert(&view3d);

    Matrix44 m;
    view3d.calc_projection_matrix(m, gfloat(img_width)/gfloat(img_height));

    return RenderParam::create(view3d.ring_model_matrix, view3d.planet_model_matrix, view3d.view_matrix, m, img_width, img_height, settings.get_antialiasing());
  }

  void RenderParam::get_ray_dir(Vector3& dir, gfloat x, gfloat y)
  {
    dir = singletonA()->inv_projection_matrix * Vector3(x, y, 0.f);
    dir = singletonA()->inv_view_matrix * Vector4(dir, 0.f);
    dir.normalize();
  }

  void RenderParam::get_camera_pos(Vector3& pos)
  {
    pos = singletonA()->inv_view_matrix * Vector3(0.f, 0.f, 0.f);
  }

  bool RenderParam::is_something_visible_within(int x, int y, int w, int h)
  {
    bool& culling = singletonA()->culling;
    gfloat& culling_epsilon = singletonA()->culling_epsilon;
    CullingCircle& bounding_sphere = singletonA()->bounding_sphere;
    Vector2* bounding_ngon = singletonA()->bounding_ngon;

    if(!culling)
      return true;

    x-=culling_epsilon;
    y-=culling_epsilon;
    w+=culling_epsilon*2;
    h+=culling_epsilon*2;

    bool visible_planet;
    bool visible_ring = false;

    visible_planet = bounding_sphere.is_within_tile(x, y, w, h, culling_epsilon);

    if(get_ring().visible && !visible_planet)
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

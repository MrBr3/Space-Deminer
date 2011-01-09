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
  void calc_pixel_color(ColorRGBA& resulting_color, const RenderParam& render_param, int x, int y)
  {
    ColorRGBA color;

    gfloat screen_rel_x  = gfloat(x)*render_param.inv_img_width;
    gfloat screen_rel_y  = gfloat(y)*render_param.inv_img_height;

    gfloat pos_x = screen_rel_x*2.f-1.f;
    gfloat pos_y = -2.f*screen_rel_y+1.f;
    gfloat hw = render_param.inv_img_width;
    gfloat hh = render_param.inv_img_height;
    gfloat qw = render_param.inv_img_width * 0.5f;
    gfloat qh = render_param.inv_img_height * 0.5f;

    ColorRGBA tmp[8];

    switch(render_param.rays_per_pixel)
    {
    case 8:
      Ray(pos_x+qw, pos_y+qh, render_param).get_color(tmp[7]);
      Ray(pos_x-qw, pos_y+qh, render_param).get_color(tmp[6]);
      Ray(pos_x-qw, pos_y-qh, render_param).get_color(tmp[5]);
      Ray(pos_x+qw, pos_y-qh, render_param).get_color(tmp[4]);
    case 4:
      Ray(pos_x+hw, pos_y, render_param).get_color(tmp[3]);
      Ray(pos_x, pos_y+hh, render_param).get_color(tmp[2]);
    case 2:
      Ray(pos_x+hw, pos_y+hh, render_param).get_color(tmp[1]);
    case 1:
      Ray(pos_x, pos_y, render_param).get_color(tmp[0]);
      break;
    default:
      g_assert_not_reached();
    }

    gfloat n_visible = 0;

    resulting_color.set(0.f, 0.f, 0.f, 0.f);
    for(gsize i=0; i<render_param.rays_per_pixel; ++i)
    {
      if(tmp[i].a>1e-6f)
      {
        n_visible+=1.f;
        gfloat a = tmp[i].a;
        resulting_color.r += tmp[i].r * a;
        resulting_color.g += tmp[i].g * a;
        resulting_color.b += tmp[i].b * a;
        resulting_color.a += tmp[i].a;
      }
    }

    if(n_visible>0.f)
    {
      gfloat inv_n_visible  = 1.f/n_visible;
      resulting_color.r *= inv_n_visible;
      resulting_color.g *= inv_n_visible;
      resulting_color.b *= inv_n_visible;
    }
    resulting_color.a /= render_param.rays_per_pixel;
  }
}
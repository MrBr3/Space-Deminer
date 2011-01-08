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

    Ray ray(screen_rel_x*2.f-1.f, -2.f*screen_rel_y+.1f, render_param);

    ray.get_color(resulting_color);
  }
}
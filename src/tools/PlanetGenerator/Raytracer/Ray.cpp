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

#include "./Raytracer.hpp"

namespace Raytracer
{
  Ray::Ray(gfloat x, gfloat y) : screen_pos(x, y)
  {
    RenderParam::get_ray_dir(dir, x, y);
    RenderParam::get_camera_pos(origin);
  }

  void Ray::get_color(ColorRGBA& resulting_color)
  {
    ColorRGBA planet_color, ring_color;
    gfloat planet_distance, ring_distance;
    bool planet_hit, ring_hit;

    resulting_color.set(0.f, 0.f, 0.f, 0.f);

    planet_hit  = RenderParam::get_planet().get_color(planet_color, screen_pos, *this, planet_distance);
    ring_hit  = RenderParam::get_ring().get_color(ring_color, *this, ring_distance);

    if(planet_hit)
    {
      if(ring_hit && ring_distance<=planet_distance)
      {
        resulting_color = overlay(ring_color, planet_color);
      }else
      {
        resulting_color = planet_color;
      }
    }else
    {
      if(ring_hit)
        resulting_color = ring_color;
    }
  }
}

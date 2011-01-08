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
  Ray::Ray(gfloat x, gfloat y, const RenderParam& render_param_) : render_param(render_param_)
  {
    dir = render_param.inv_view_matrix*Vector4(x*render_param.aspect, y, -1.f, 0.f);//render_param.inv_projection_matrix * render_param.inv_view_matrix  * Vector4(x, y, 1.f, 1.f);
    dir.normalize();
    origin = render_param.inv_view_matrix * Vector3(0.f, 0.f, 0.f);
  }

  void Ray::get_color(ColorRGBA& resulting_color)
  {
    render_param.planet.get_color(resulting_color, *this);
  }

  void Ray::transform(const Matrix44& m)
  {
    dir = m * dir;
    origin  = m * origin;
  }
}
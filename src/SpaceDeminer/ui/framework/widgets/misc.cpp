/* Space Deminer is a small 2D Arcade Game. Your task ist to eliminate
 * intelligent Mines created in the last galactic war.
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

#include "./misc.hpp"

namespace Framework
{
  Misc::Misc()
  {
    _xalign  = .5f;
    _yalign  = .5f;
    set_name("misc");

    _back_r = 0.f;
    _back_g = 0.f;
    _back_b = 0.f;
    _back_a = 0.f;
  }

  Misc::~Misc()throw()
  {
  }

  void Misc::set_alignment(Real xalign, Real yalign)
  {
    _xalign  = xalign;
    _yalign  = yalign;

    invalidate();
  }

  void Misc::set_back_color(Real r, Real g, Real b, Real a)
  {
    _back_r=r, _back_g=g, _back_b=b, _back_a=a;
  }

  void Misc::on_expose(EventExpose& paint_tool)
  {
    if(_back_a)
      paint_tool.draw_color_rect(0, 0, get_width(), get_height(), _back_r, _back_g, _back_b, _back_a);
  }
}
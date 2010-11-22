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

#ifndef _SPACEDEMINER_UI_FRAMEWORK_MISC_H_
#define _SPACEDEMINER_UI_FRAMEWORK_MISC_H_

#include "./../widget.hpp"

namespace Framework
{
  class Misc : public Widget
  {
  public:
    void set_alignment(Real xalign=0.f, Real yalign=0.5f);
    void get_alignment(Real& xalign, Real& yalign)const throw()
    {
      xalign = _xalign;
      yalign = _yalign;
    }
    Real get_xalign()const throw(){return _xalign;}
    Real get_yalign()const throw(){return _yalign;}

    void on_expose(EventExpose& paint_tool);

    void set_back_color(Real r, Real g, Real b, Real a=1.f);

    Misc();
    ~Misc()throw();

  private:
    Real _xalign, _yalign;
    Real _back_r, _back_g, _back_b, _back_a;
  };
}

#endif
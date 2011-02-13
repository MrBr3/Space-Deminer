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

#ifndef _SPACEDEMINER_UI_FRAMEWORK_ALIGNMENT_H_
#define _SPACEDEMINER_UI_FRAMEWORK_ALIGNMENT_H_

#include "./bin.hpp"

namespace Framework
{
  class Alignment : public Bin
  {
  public:
    /**
     * \param xalign the x position relative to it's parents client area, from  to 1 (0 means leftmost 1 means rightmost)
     * \param yalign the y position relative to it's parents client area (0 means bottommost 1 means topmost)
     * \param xscale
     * \param yscale
     * \param amount_expands
     *        \li if true (default) <tt>x/yscale</tt> will be interpreted as the amount, the unused (width or height minus size_request) horizontal/vetical space will be used to enlarge the widget.
     *        \li if false <tt>x/yscale</tt> will be interpreted as the amount, the full horizontal/vetical space will for the widget.
     * */
    void set(Real xalign, Real yalign, Real xscale, Real yscale, bool amount_expands=true)
    {
      _xalign = xalign;
      _yalign = yalign;
      _xscale = CLAMP(xscale, 0.f, 1.f);
      _yscale = CLAMP(yscale, 0.f, 1.f);
      _amount_expands = amount_expands;
      invalidate(); // TODO TEST, whether to use rearrange_children here?
    }

    void set_padding(int padding_top, int padding_bottom, int padding_left, int padding_right)
    {
      _padding_top  = padding_top;
      _padding_bottom = padding_bottom;
      _padding_left  = padding_left;
      _padding_right = padding_right;
      recalc_size_request(); // TODO TEST, whether to use rearrange_children here?
    }

    void on_size_request(int& w, int& h);

    void add(Widget& widget)throw(IllegalParam, BinHasAlreadyChild, WidgetHasAlreadyParent);

    Alignment();
    ~Alignment()throw();

  protected:

    void v_rearrange_children();

  private:
    Real _xalign, _yalign, _xscale, _yscale;
    int _padding_top, _padding_bottom, _padding_left, _padding_right;
    bool _amount_expands;
  };
}

#endif
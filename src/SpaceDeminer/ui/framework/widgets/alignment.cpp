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

#include "./alignment.hpp"

namespace Framework
{
  Alignment::Alignment()
  {
    _xalign = 0.5f;
    _yalign = 0.5f;
    _xscale = 1.f;
    _yscale = 1.f;
    _padding_top  = _padding_bottom = _padding_left = _padding_right = 0;

    set_name("alignment");
  }

  Alignment::~Alignment()throw()
  {
  }

  void Alignment::v_rearrange_children()
  {
    g_assert(get_child());

    if(!get_child()->is_visible())
      return;

    int left_padding   = get_border_width()+_padding_left;
    int right_padding  = get_border_width()+_padding_right;
    int top_padding    = get_border_width()+_padding_top;
    int bottom_padding = get_border_width()+_padding_bottom;

    int w = get_width()-left_padding-right_padding;
    int h = get_height()-top_padding-bottom_padding;

    int child_width  = round((w-get_child()->get_size_request_width()) *_xscale) + get_child()->get_size_request_width();
    int child_height = round((h-get_child()->get_size_request_height())*_yscale) + get_child()->get_size_request_height();

    set_child_allocation(*get_child(),
      left_padding + round((w-child_width) *_xalign),
      top_padding  + round((h-child_height)*_yalign),
        child_width,
        child_height);
  }

  void Alignment::on_size_request(int& w, int& h)
  {
    Container::on_size_request(w, h);

    w += _padding_left+_padding_right;
    h += _padding_top+_padding_bottom;

    if(get_child() && get_child()->is_visible())
    {
      w+=get_child()->get_size_request_width();
      h+=get_child()->get_size_request_height();
    }
  }

  void Alignment::add(Widget& widget)throw(IllegalParam, BinHasAlreadyChild, WidgetHasAlreadyParent)
  {
    Bin::add(widget);
  }
}
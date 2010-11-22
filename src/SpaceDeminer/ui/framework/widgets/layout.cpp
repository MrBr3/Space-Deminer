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

#include "layout.hpp"

namespace Framework
{
  Layout::Layout()
  {
    set_name("layout");
  }

  Layout::~Layout()throw()
  {
  }

  void Layout::add(Widget& widget, const Gdk::Rectangle& rect)
  {
    validate_child_(widget);

    Allocation& alloc = const_cast<Allocation&>(widget.get_allocation());

    alloc = rect;

    Container::add_child_(widget);
  }

  void Layout::on_expose(EventExpose& paint_tool)
  {
    paint_tool.push_offset(get_border_width(), get_border_width());
    Container::on_expose(paint_tool);
    paint_tool.pop_offset();
  }

  void Layout::on_debug_draw(EventExpose& paint_tool)const
  {
    paint_tool.push_offset(get_border_width(), get_border_width());
    Container::draw_childrens_debug(paint_tool);
    paint_tool.pop_offset();
    Widget::on_debug_draw(paint_tool);
  }

  void Layout::v_rearrange_children()
  {
  }

  void Layout::on_size_request(int& w, int& h)
  {
    Container::on_size_request(w, h);

    int max_left=0, max_bottom=0;

    const std::list<Widget*>& children  = get_children();
    for(std::list<Widget*>::const_iterator c_iter = children.begin(); c_iter != children.end(); ++c_iter)
    {
      Widget* child = *c_iter;

      g_assert(child);

      max_left   = MAX(child->get_allocation().get_x()+child->get_width (), max_left);
      max_bottom = MAX(child->get_allocation().get_y()+child->get_height(), max_bottom);
    }

    w += max_left;
    h += max_bottom;
  }
}
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

#include "./windowframe.hpp"

namespace Framework
{
  WindowFrame::WindowFrame()
  {
    child_dist.set(0);

    set_name("window-frame");
  }

  WindowFrame::~WindowFrame()throw()
  {
  }

  void WindowFrame::set_theme(const Glib::RefPtr<Theme>& theme)
  {
    Bin::set_theme(theme);
    if(theme)
      theme->get_metrics("child-dist/Window/Frame/"+get_name(), child_dist);
  }

  void WindowFrame::on_size_request(int& w, int& h)
  {
    Bin::on_size_request(w, h);

    w += child_dist.x1+child_dist.x2;
    h += child_dist.y1+child_dist.y2;
  }

  void WindowFrame::v_rearrange_children()
  {
    if(!get_child())
      return;

    set_child_allocation(*get_child(), get_border_width()+child_dist.x1, get_border_width()+child_dist.y1, MAX(0,get_width()-get_border_width()*2-child_dist.x1-child_dist.x2), MAX(0,get_height()-get_border_width()*2-child_dist.y1-child_dist.y2));
  }

  void WindowFrame::on_expose(EventExpose& paint_tool)
  {
    paint_tool.draw_widget_back("Window/Frame", 0, *this);

    Container::on_expose(paint_tool);
  }
}
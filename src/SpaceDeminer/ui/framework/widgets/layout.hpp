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

#ifndef _SPACEDEMINER_UI_FRAMEWORK_WIDGET_LAYOUT_H_
#define _SPACEDEMINER_UI_FRAMEWORK_WIDGET_LAYOUT_H_

#include "./container.hpp"

namespace Framework
{
  class Layout : public Container
  {
  public:
    void v_rearrange_children();

    void add(Widget& widget, const Gdk::Rectangle& rect);
    void add(Widget& widget, int x, int y, int width, int height)
    {
      add(widget, Gdk::Rectangle(x, y, width, height));
    }

    void on_expose(EventExpose& paint_tool);
    void on_debug_draw(EventExpose& paint_tool)const;
    void on_size_request(int& w, int& h);

    Layout();
    ~Layout()throw();
  };
}

#endif
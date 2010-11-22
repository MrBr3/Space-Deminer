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

#include "./dummy-widget.hpp"

namespace Framework
{
  DummyWidget::DummyWidget()
  {
    set_color();
    set_name("dummy-widget");
  }

  DummyWidget::~DummyWidget()throw()
  {
  }

  void DummyWidget::on_expose(EventExpose& paint_tool)
  {
    paint_tool.draw_color_rect(0, 0, get_width(), get_height(), color.r, color.g, color.b, color.a);
  }

  void DummyWidget::set_color(guint32 hex)
  {
    color.r = Real((hex&0xff000000)>>24)/255.f;
    color.g = Real((hex&0x00ff0000)>>16)/255.f;
    color.b = Real((hex&0x0000ff00)>> 8)/255.f;
    color.a = Real( hex&0x000000ff     )/255.f;
  }
}
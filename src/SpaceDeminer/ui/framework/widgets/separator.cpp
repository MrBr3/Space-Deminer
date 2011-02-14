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

#include "./separator.hpp"

namespace Framework
{
  Separator::Separator()
  {
    set_name("separator");

    _width  = 0;
  }

  Separator::~Separator()throw()
  {
  }

// ========

  HSeparator::HSeparator()
  {
    set_name("hseparator");
  }

  HSeparator::~HSeparator()throw()
  {
  }

  void HSeparator::set_theme(const Glib::RefPtr<Theme>& theme)
  {
    ParentClass::set_theme(theme);

    Theme::Metrics width;

    if(theme)
      theme->get_metrics("width/Separator/HSeparator/"+get_name(), width);

    this->_width = abs(width.y2-width.y1);
  }

  void HSeparator::on_size_request(int& w, int& h)
  {
    w = 0;
    h = this->_width;
  }

  void HSeparator::on_expose(EventExpose& paint_tool)
  {
    paint_tool.draw_widget_back("Separator/HSeparator/"+get_name(), 0, *this);
  }

// ========

  VSeparator::VSeparator()
  {
    set_name("vseparator");
  }

  VSeparator::~VSeparator()throw()
  {
  }

  void VSeparator::set_theme(const Glib::RefPtr<Theme>& theme)
  {
    ParentClass::set_theme(theme);

    Theme::Metrics width;

    if(theme)
      theme->get_metrics("width/Separator/VSeparator/"+get_name(), width);

    this->_width = abs(width.x2-width.x1);
  }

  void VSeparator::on_size_request(int& w, int& h)
  {
    w = this->_width;
    h = 0;
  }

  void VSeparator::on_expose(EventExpose& paint_tool)
  {
    paint_tool.draw_widget_back("Separator/VSeparator/"+get_name(), 0, *this);
  }
}
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

#include "MainWindow.hpp"

ViewSettings* view_settings = nullptr;

ViewSettings::ViewSettings()
{
  g_assert(!view_settings);
  view_settings = this;

  _back_color_red = 0.5f;
  _back_color_green = 0.5f;
  _back_color_blue = 0.5f;

  set_main_caption(_("View Settings"));

  append_color_widget("view-back-color", _("Back Color"), "The Color of the background in the 3D view", sigc::mem_fun(*this, &ViewSettings::get_back_color), sigc::mem_fun(*this, &ViewSettings::set_back_color), signal_back_color_changed());
}

ViewSettings::~ViewSettings()throw()
{
  g_assert(view_settings);
  view_settings = nullptr;
}

Gdk::Color ViewSettings::get_back_color()const
{
  Gdk::Color color;
  color.set_rgb_p(_back_color_red, _back_color_green, _back_color_blue);
  return color;
}

void ViewSettings::set_back_color(const Gdk::Color& color)
{
  _back_color_red = color.get_red_p();
  _back_color_green = color.get_green_p();
  _back_color_blue = color.get_blue_p();

  _signal_back_color_changed.emit();
}
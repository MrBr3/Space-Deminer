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

MainWindow* main_window  = nullptr;

MainWindow::MainWindow()
{
  add(_vbox);
    _vbox.show();
    _vbox.pack_start(_view_hbox);
      _view_hbox.show();
      _view_hbox.pack_start(view_3d);

  set_default_size(480, 360);
  set_title(_("SphereUVAssistant"));

  g_assert(!main_window);
  main_window = this;

  show();
}

MainWindow::~MainWindow()throw()
{
  view_3d.deinit();

  main_window = nullptr;
}
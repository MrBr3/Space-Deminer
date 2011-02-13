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

#include "./../menu.hpp"

MainMenuWindow::MainMenuWindow()
{
  add(alignment);
  alignment.add(vbox);
  alignment.set(0.5f, 0.8f, 0.52f, 0.4444f, false);

  vbox.pack_start(l_caption, false, false);
  vbox.pack_start(hsep1, false, false, vsep.get_size_request_height());
  vbox.pack_end(hsep2, false, false, vsep.get_size_request_height());
  vbox.pack_end(hbox, false, false);

  hbox.pack_start(sep1);
  hbox.pack_start(btn_options, false, false);
  hbox.pack_start(vsep, false, false, vsep.get_size_request_width());
  hbox.pack_start(btn_about, false, false);
  hbox.pack_start(sep2);

  btn_options.set_label(_("_Options"));
  btn_about.set_label(_("_About"));

  show_all_children();
}

MainMenuWindow::~MainMenuWindow()throw()
{
}

void MainMenuWindow::set(const Glib::ustring& caption, Widget& child)
{
  l_caption.set_label(caption);
  vbox.pack_start(child);
}

// ========

MainMenu::MainMenu()
{
  set(_("MainMenu"), vbox);

  vbox.pack_start(btn_run);
  vbox.pack_start(btn_highscore);
  vbox.pack_start(btn_quit);

  btn_run.set_label(_("Start Game"));
  btn_highscore.set_label(_("Show Highscore"));
  btn_quit.set_label(_("Quit Game"));

  vbox.show();
  vbox.show_all_children();
}

MainMenu::~MainMenu()throw()
{
}

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
   add(vbox);
   
   vbox.pack_start(l_caption, false, false);
   //vbox.pack_start(sep1, false, false);
   //vbox.pack_end(sep2, false, false);
   vbox.pack_end(hbox, false, false);
   
   hbox.pack_start(btn_options, false, false);
   //hbox.pack_start(vsep, false, false);
   hbox.pack_start(btn_about, false, false);
   
   btn_options.set_label(_("_Options"));
   btn_about.set_label(_("_About"));
   
   show_all_children();
 }
 
 MainMenuWindow::~MainMenuWindow()throw()
 {
 }
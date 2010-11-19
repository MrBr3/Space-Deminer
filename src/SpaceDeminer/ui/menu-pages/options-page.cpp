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

#include "./../menu.hpp"

class MenuPage_Options : public MenuPage
{
public:
  Framework::VBox vbox;

  MenuPage_Options()
  {
    parent_page_id  = MENU_PAGE_MAIN;

    set_main_widget(&vbox);
    set_icon(Framework::Image::create_from_file("$(ui-menu-icon-path)/configure.png"));
  }
  ~MenuPage_Options()throw()
  {
  }
};

void register_menu_page_options()
{
  Menu::register_menu_page(_("Options"), MENU_PAGE_OPTIONS, new MenuPage_Options);
}
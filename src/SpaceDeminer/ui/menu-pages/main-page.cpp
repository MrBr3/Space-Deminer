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

class MenuPage_Main : public MenuPage
{
public:
  Framework::VButtonBox entries_vbox;

  void on_open_first_time()throw()
  {
    try
    {
      entries_vbox.set_name("main-menupage entries_vbox");
      entries_vbox.pack_start(create_submenu_button(MENU_PAGE_OPTIONS));
      entries_vbox.pack_start(create_submenu_button(MENU_PAGE_HIGHSCORE));
      entries_vbox.pack_start(create_submenu_button(MENU_PAGE_QUIT));
    }catch(...)
    {
      g_warning("Couldn't create MainMenu");
    }
  }
  MenuPage_Main()
  {
    entries_vbox.set_border_width(SPACING_LARGE);
    entries_vbox.set_spacing(SPACING_LARGE);
    entries_vbox.set_layout(Framework::BUTTONBOX_SPREAD);

    set_main_widget(&entries_vbox);
    set_icon(Framework::Image::create_from_file("$(ui-menu-icon-path)/go-home.png"));
  }
  ~MenuPage_Main()throw()
  {
  }
};

void register_menu_page_main()
{
  Menu::register_menu_page(_("Main Menu"), MENU_PAGE_MAIN, new MenuPage_Main);
}

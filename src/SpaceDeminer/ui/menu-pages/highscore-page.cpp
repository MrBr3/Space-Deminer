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
#include "./../../game/difficulty.hpp"

class MenuPage_Highscore : public MenuPage
{
public:
  Framework::Notebook notebook;

  MenuPage_Highscore()
  {
    parent_page_id  = MENU_PAGE_MAIN;

    set_align_size_x(0.2);
    set_align_size_y(0.5);

    set_main_widget(&notebook);
    set_icon(Framework::Image::create_from_file("$(ui-menu-icon-path)/games-highscores.png"));

    notebook.show();
  }
  ~MenuPage_Highscore()throw()
  {
  }
};

void register_menu_page_highscore()
{
  Menu::register_menu_page(_("Highscore"), MENU_PAGE_HIGHSCORE, new MenuPage_Highscore);
}
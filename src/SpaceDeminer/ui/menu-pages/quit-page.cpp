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
#include <gtkmm/main.h>

class MenuPage_Quit : public MenuPage
{
public:
  Framework::Label label;
  Framework::VBox main_vbox;
  Framework::HButtonBox hb_answers;
  Framework::Button btn_quit, btn_back_to_main_menu;
  Framework::Alignment center;

  MenuPage_Quit()
  {
    center.add(main_vbox);
    center.set(.5, .5, 0., 0.);
      main_vbox.show();
      main_vbox.set_spacing(SPACING_LARGE);
      main_vbox.pack_start(label, false, false);
        label.set_label(_("Do you really wan't to quit the game?"));
        label.show();
      main_vbox.pack_start(hb_answers, false, false);
        hb_answers.show();
        hb_answers.set_layout(Framework::BUTTONBOX_SPREAD);
        hb_answers.pack_start(btn_quit, false, false);
          btn_quit.show();
          btn_quit.set_label(_("_Yes"));
          btn_quit.set_tooltip_text(_("Yes I have enough, so close finally this game now"));
          btn_quit.signal_clicked().connect(sigc::ptr_fun(&Gtk::Main::quit));
        hb_answers.pack_start(btn_back_to_main_menu, false, false);
          btn_back_to_main_menu.show();
          btn_back_to_main_menu.set_label(_("_No"));
          btn_back_to_main_menu.set_tooltip_text(_("No, I wan't want to go back to the Main Menu"));
          btn_back_to_main_menu.signal_clicked().connect(sigc::bind(sigc::ptr_fun(&Menu::activate_menu_page), MENU_PAGE_MAIN));

    set_main_widget(&center);
    set_title_widget(nullptr);
    set_icon(Framework::Image::create_from_file("$(ui-menu-icon-path)/window-close.png"));
  }

  ~MenuPage_Quit()throw()
  {
  }
};

void register_menu_page_quit()
{
  Menu::register_menu_page(_("Quit"), MENU_PAGE_QUIT, new MenuPage_Quit);
}
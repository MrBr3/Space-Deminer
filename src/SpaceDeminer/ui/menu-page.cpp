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

#include "./menu.hpp"
#include "./main-window.hpp"
#include <iostream>

MenuPage::MenuPage()throw()
{
  _id = MENU_PAGE_INVALID;
  parent_page_id = MENU_PAGE_INVALID;

  _main_widget  = nullptr;
  _title_widget = &_default_title;

  _default_title.set_spacing(SPACING_LARGE);
  _default_title.show();
  _default_title.pack_start(_dt_btn_back_align, false, false);
    _dt_btn_back_align.show();
    _dt_btn_back_align.set(.5, .5, 0., 0.);
    _dt_btn_back_align.add(_dt_btn_back);
      _dt_btn_back.set_tooltip_text(_("Go back"));
      _dt_btn_back.show();
      _dt_btn_back.set_relief(Framework::RELIEF_OVER);
  _default_title.pack_start(_dt_page_label);
    _dt_page_label.show();
    _dt_page_label.set_alignment(0., .5);

  _align_size_x = 0.;
  _align_size_y = 0.;

  _is_first_opening = true;
}

MenuPage::~MenuPage()throw()
{
}

Framework::Widget& MenuPage::create_submenu_button(MenuPageID id)throw(Glib::Exception)
{
  Glib::RefPtr<MenuPage> page = Menu::get_menu_page(id);

  Framework::Button* btn  = new Framework::Button(page->caption);

  btn->manage();

  btn->signal_clicked().connect(sigc::bind(sigc::ptr_fun(&Menu::activate_menu_page), id));
  btn->show();

  return *btn;
}

void MenuPage::init()throw()
{
  try
  {
    if(_title_widget != &_default_title)
      return;

    if(parent_page_id==MENU_PAGE_INVALID)
      _dt_btn_back.hide();
    else
    {
      Glib::RefPtr<MenuPage> parent = Menu::get_menu_page(parent_page_id);

      _dt_btn_back.set_tooltip_text(Glib::ustring::compose(_("Go back to %1"), parent->caption));
      _dt_btn_back.signal_clicked().connect(sigc::bind(sigc::ptr_fun(Menu::activate_menu_page), parent_page_id));
      _dt_btn_back.set_image(Menu::get_go_back_image());

    }

    _dt_page_label.set_label(caption);
  }CATCH_ALL("**MenuPage::init** ", NOTHING_MACRO)

  on_open_first_time();
  _is_first_opening  = false;
}
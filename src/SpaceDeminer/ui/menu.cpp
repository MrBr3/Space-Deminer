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
#include <gtkmm/main.h>
#include <iostream>

void register_menu_page_main();
  void register_menu_page_options();
  void register_menu_page_highscore();
  void register_menu_page_quit();

Menu::Menu()
{
  g_assert(MENU_PAGE_MAIN!=MENU_PAGE_HIGHSCORE);

  MainWindow::get_window_manager()->register_window(0, menu_window);
  menu_window.show();

  register_menu_page_main();
    register_menu_page_options();
    register_menu_page_highscore();
    register_menu_page_quit();
}

Menu::~Menu()throw()
{
  g_assert(MenuAlignment::get_singleton());
  if(MenuAlignment::get_singleton()->get_child())
    MenuAlignment::get_singleton()->remove();
}

Framework::ResPtr<Framework::Image> Menu::get_go_back_image()
{
  if(!get_singleton()->_go_back_image)
    get_singleton()->_go_back_image = Framework::Image::create_from_file("$(ui-menu-icon-path)/../32/go-previous.png");
  return get_singleton()->_go_back_image;
}

Glib::RefPtr<MenuPage> Menu::get_menu_page(MenuPageID id)throw(MenuPage::InvalidID, UnknownID)
{
  if(id==MENU_PAGE_INVALID)
    throw MenuPage::InvalidID();

  MenuPageMap::iterator iter  = get_singleton()->_menu_pages.find(id);

  if(iter==get_singleton()->_menu_pages.end())
    throw UnknownID(id);

  return iter->second;
}

void Menu::activate_menu_page(MenuPageID id)throw(UnknownID)
{
  g_assert(MenuAlignment::get_singleton());
  Glib::RefPtr<MenuPage> page;

  try
  {
    page= get_menu_page(id);
  }catch(MenuPage::InvalidID)
  {
  }

  if(get_singleton()->_active_page)
  {
    get_singleton()->_active_page->on_close();
    MenuAlignment::get_singleton()->main_area.remove_if_child();
    MenuAlignment::get_singleton()->link_area.remove_if_child();
    MenuAlignment::get_singleton()->page_icon.clear();
  }else
  {
    g_assert(!MenuAlignment::get_singleton()->main_area.get_child());

    MenuAlignment::get_singleton()->show();
  }

  get_singleton()->_active_page  = page;

  if(get_singleton()->_active_page)
  {
    if(get_singleton()->_active_page->_is_first_opening)
      get_singleton()->_active_page->init();

    get_singleton()->_active_page->on_open();

    MenuAlignment::get_singleton()->set(.5, .5, page->_align_size_x, page->_align_size_y);

    if(get_singleton()->_active_page->_main_widget)
    {
      MenuAlignment::get_singleton()->main_area.add(*get_singleton()->_active_page->_main_widget);
      get_singleton()->_active_page->_main_widget->show();
    }
    if(get_singleton()->_active_page->_title_widget)
    {
      MenuAlignment::get_singleton()->link_area.add(*get_singleton()->_active_page->_title_widget);
      get_singleton()->_active_page->_title_widget->show();
    }
    if(get_singleton()->_active_page->_icon)
      MenuAlignment::get_singleton()->page_icon.set(get_singleton()->_active_page->_icon);
  }else
    MenuAlignment::get_singleton()->hide();
}

void Menu::register_menu_page(const Glib::ustring& caption, MenuPageID id, MenuPage* men_page)throw(MenuPage::InvalidID, IDAlreadyGiven, IllegalParam, UnknownID)
{
  if(id==MENU_PAGE_INVALID)
    throw MenuPage::InvalidID();
  if(!men_page)
    throw IllegalParam("men_page", "Must not be nullptr");
  if(men_page->get_id()!=MENU_PAGE_INVALID)
    throw IllegalParam("men_page", "is already registered");

  MenuPageMap::iterator iter  = get_singleton()->_menu_pages.find(id);
  Glib::RefPtr<MenuPage> page = Glib::RefPtr<MenuPage>(men_page);

  if(iter!=get_singleton()->_menu_pages.end())
    throw IDAlreadyGiven(id);

  get_singleton()->_menu_pages.insert(std::make_pair(id, page));

  men_page->_id  = id;
  men_page->caption  = caption;

  men_page->on_registered();
}
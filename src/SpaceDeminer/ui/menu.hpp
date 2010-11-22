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

#ifndef _SPACE_DEMINER_MENU_H_
#define _SPACE_DEMINER_MENU_H_

#include <map>

#include <base.hpp>
#include "./../options.hpp"

#include "./menu-back.hpp"
#include "./menu-frame.hpp"
#include "./menu-page.hpp"

#include "./menu-alignment.hpp"

#define SPACING_SMALL 2
#define SPACING_NORMAL 4
#define SPACING_LARGE 4

class Menu : public StaticManager<Menu>
{
public:
  class NoMenuPageRegistered : public Glib::Exception
  {
  public:
    Glib::ustring what()const{return "No Menu Page registered yet.";}

    ~NoMenuPageRegistered()throw(){}
  };
  class IDAlreadyGiven : public Glib::Exception
  {
  public:
    const gsize _id;
    Glib::ustring what()const{return Glib::ustring::compose("The desired MenuPage ID (%1) is already given.", _id);}

    IDAlreadyGiven(gsize id) : _id(id){}
    ~IDAlreadyGiven()throw(){}
  };
  class UnknownID : public Glib::Exception
  {
  public:
    const gsize _id;
    Glib::ustring what()const{return Glib::ustring::compose("Unkown MenuPage ID %1.", _id);}

    UnknownID(gsize id) : _id(id){}
    ~UnknownID()throw(){}
  };
  class NoWidget : public Glib::Exception
  {
  public:
    Glib::ustring what()const{return "No Widget";}

    NoWidget(){}
    ~NoWidget()throw(){}
  };

  static void register_menu_page(const Glib::ustring& caption, MenuPageID id, MenuPage* men_page)throw(MenuPage::InvalidID, IDAlreadyGiven, IllegalParam, UnknownID);

  static Glib::RefPtr<MenuPage> get_menu_page(MenuPageID id)throw(MenuPage::InvalidID, UnknownID);
  static Glib::RefPtr<MenuPage> get_active_menu_page()throw(NoMenuPageRegistered)
  {
    if(!get_singleton()->_active_page)
      throw NoMenuPageRegistered();
    return get_singleton()->_active_page;
  }

  /**
   * \param id - \c MENU_PAGE_INVALID is allowed here
   * */
  static void activate_menu_page(MenuPageID id)throw(UnknownID);

  static Framework::ResPtr<Framework::Image> get_go_back_image();

  Menu();
  ~Menu()throw();

  Framework::FullscreenWindow menu_window;

private:
  typedef std::map<MenuPageID, Glib::RefPtr<MenuPage> > MenuPageMap;

  MenuPageMap _menu_pages;
  Glib::RefPtr<MenuPage> _active_page;

  Framework::ResPtr<Framework::Image> _go_back_image;
};

#endif
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

#ifndef _SPACE_DEMINER_MENU_PAGE_H_
#define _SPACE_DEMINER_MENU_PAGE_H_

#include <base.hpp>
#include "./framework.hpp"

enum MenuPageID
{
  MENU_PAGE_INVALID=-1,

  MENU_PAGE_MAIN  = 0,
    MENU_PAGE_OPTIONS,
    MENU_PAGE_HIGHSCORE,
    MENU_PAGE_QUIT,
};

class MenuPage;

class MenuPage : public Refable
{
public:
  class InvalidID : public Glib::Exception
  {
  public:
    Glib::ustring what()const{return _("Invalid MenuPage ID");}

    ~InvalidID()throw(){}
  };

  MenuPageID parent_page_id;
  Glib::ustring caption;

  /** \brief Gets the ID of the Page.
   *
   * \return the requested id.
   * */
  MenuPageID get_id()const throw(){return _id;}

  /** \brief Overidable getting called, when opening the page.
   * */
  virtual void on_open()throw(){}

  /** \brief Overidable getting called, when opening the pag the first time.
   * */
  virtual void on_open_first_time()throw(){}

  /** \brief Overidable getting called, when closing the page.
   *
   * The default implementation does nothing.
   * */
  virtual void on_close()throw(){}

  /** \brief Overidable getting called, when the page gets registered.
   *
   * The default implementation does nothing.
   * */
  virtual void on_registered()throw(){}

  /** \brief Creates a simple Button, doing nothing that opening a menu.
   *
   * \return a reference to the new button - thew caller owns it (but gtk::manage is automatically called)
   * */
  Framework::Widget& create_submenu_button(MenuPageID id)throw(Glib::Exception);

protected:
  MenuPage()throw();
  ~MenuPage()throw();

  void set_main_widget(Framework::Widget* w)throw(){_main_widget=w;}
  void set_title_widget(Framework::Widget* w)throw(){_title_widget=w;}
  void set_icon(const Glib::RefPtr<Framework::Image>& img)throw(){_icon=img;}

  void set_align_size_x(gdouble s){_align_size_x  = CLAMP(s, 0., 1.);}
  void set_align_size_y(gdouble s){_align_size_y  = CLAMP(s, 0., 1.);}

private:
  MenuPageID _id;

  Framework::Widget* _main_widget;
  Framework::Widget* _title_widget;
  Glib::RefPtr<Framework::Image> _icon;

  Framework::HBox _default_title;
  Framework::Alignment _dt_btn_back_align;
  Framework::Button _dt_btn_back;
  Framework::Label _dt_page_label;

  gdouble _align_size_x;
  gdouble _align_size_y;

  void init()throw();

  bool _is_first_opening;

  friend class Menu;
};

#endif
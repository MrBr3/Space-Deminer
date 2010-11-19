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

#ifndef _SPACEDEMINER_UI_FRAMEWORK_VIEWPORT_H_
#define _SPACEDEMINER_UI_FRAMEWORK_VIEWPORT_H_

#include "./bin.hpp"
#include <gtkmm/adjustment.h>

namespace Framework
{
  class ScrollableWidget
  {
  public:
    virtual void on_scrolled()=0;

    const Gtk::Adjustment& get_hadjustment()const throw(){return _hadjustment;}
    const Gtk::Adjustment& get_vadjustment()const throw(){return _vadjustment;}

    Gtk::Adjustment& get_hadjustment()throw(){return _hadjustment;}
    Gtk::Adjustment& get_vadjustment()throw(){return _vadjustment;}

    ScrollableWidget(const Gtk::Adjustment& hadjust, const Gtk::Adjustment& vadjust) :
      _hadjustment(hadjust.get_value(), hadjust.get_lower(), hadjust.get_upper(), hadjust.get_step_increment(), hadjust.get_page_increment(), hadjust.get_page_size()),
      _vadjustment(vadjust.get_value(), vadjust.get_lower(), vadjust.get_upper(), vadjust.get_step_increment(), vadjust.get_page_increment(), vadjust.get_page_size())
    {
    }

  private:
    Gtk::Adjustment _hadjustment, _vadjustment;
  };

  class Viewport : public Bin, public ScrollableWidget
  {
  public:
    void add(Widget& widget);

    void set_theme(const Glib::RefPtr<Theme>& theme);

    void v_rearrange_children();

    void on_expose(EventExpose& paint_tool);

    void on_child_visibility_changed(Widget& child);
    void on_size_request(int& w, int& h);
    void on_scrolled();

    Viewport();
    ~Viewport()throw();

  protected:
    Theme::Metrics shadow_child_dist;
  };
}

#endif
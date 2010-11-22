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

#include "./scrolledwindow.hpp"

namespace Framework
{
  ScrolledWindow::ScrolledWindow()
  {
    _hpolicy  = POLICY_ALWAYS;
    _vpolicy  = POLICY_ALWAYS;
    _shadow_type  = SHADOW_NONE;
    set_name("scrolledwindow");
  }

  ScrolledWindow::~ScrolledWindow()throw()
  {
  }

  void ScrolledWindow::v_rearrange_children()
  {
    if(!get_child())
      return;

    set_child_allocation(*get_child(), 0, 0, get_width(), get_height());
  }

  void ScrolledWindow::add(Widget& widget)
  {
    ScrollableWidget* scrollable_widget = dynamic_cast<ScrollableWidget*>(&widget);

    if(!scrollable_widget)
    {
      _private_viewport = Glib::RefPtr<Viewport>(new Viewport);
    }

    Bin::add(widget);
  }

  void ScrolledWindow::remove(Widget& w)
  {
    if(_private_viewport)
    {
      if(&w==_private_viewport->get_child())
      {
        _private_viewport.reset();
        return;
      }else
      {
        if(_private_viewport.operator->()!=&w)
          throw Container::NotAChild();
      }
    }

    Bin::remove(w);
  }

  void ScrolledWindow::on_expose(EventExpose& paint_tool)
  {
    Bin::on_expose(paint_tool);
  }

  void ScrolledWindow::on_size_request(int& w, int& h)
  {
    w = 64;
    h = 64;
  }

  void ScrolledWindow::set_shadow_type(ShadowType shadow_type)throw(BadEnum)
  {
    if(!is_valid_enum_value(shadow_type))
      throw BadEnum();

    _shadow_type  = shadow_type;

    recalc_size_request();
    invalidate();
  }

  void ScrolledWindow::set_policy(PolicyType hpolicy, PolicyType vpolicy)throw(BadEnum)
  {
    if(!is_valid_enum_value(hpolicy))
      throw BadEnum();
    if(!is_valid_enum_value(vpolicy))
      throw BadEnum();

    _hpolicy  = hpolicy;
    _vpolicy  = vpolicy;

    recalc_size_request();
    invalidate();
  }
}
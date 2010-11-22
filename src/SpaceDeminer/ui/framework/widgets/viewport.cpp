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

#include "./viewport.hpp"

namespace Framework
{
  Viewport::Viewport() : ScrollableWidget(Gtk::Adjustment(0., 0., 1.), Gtk::Adjustment(0., 0., 1.))
  {
    shadow_child_dist.set(0);
    set_name("viewport");
  }

  Viewport::~Viewport()throw()
  {
  }

  void Viewport::set_theme(const Glib::RefPtr<Theme>& theme)
  {
    Bin::set_theme(theme);
    if(theme)
      theme->get_metrics("child-dist/SHADOW_IN/Viewport", shadow_child_dist);
  }

  void Viewport::add(Widget& widget)
  {
    Bin::add(widget);
  }

  void Viewport::on_size_request(int& w, int& h)
  {
    w = 0;
    h = 0;
  }

  void Viewport::v_rearrange_children()
  {
    if(!get_child())
      return;

    set_child_allocation(*get_child(), 0, 0, get_width(), get_height());
  }

  void Viewport::on_child_visibility_changed(Widget& child)
  {
    get_hadjustment().set_lower(0.);
    get_vadjustment().set_lower(0.);
    if(!get_child() || !get_child()->is_visible())
    {
      get_hadjustment().set_upper(0.);
      get_vadjustment().set_upper(0.);
    }else
    {
      get_hadjustment().set_upper(MAX(get_child()->get_size_request_width() -(get_width() -get_border_width()*2/*-shadow_child_dist.x1-shadow_child_dist.x2*/), 0.));
      get_vadjustment().set_upper(MAX(get_child()->get_size_request_height()-(get_height()-get_border_width()*2/*-shadow_child_dist.y1-shadow_child_dist.y2*/), 0.));
    }
  }

  void Viewport::on_expose(EventExpose& paint_tool)
  {
    Bin::on_expose(paint_tool);
  }

  void Viewport::on_scrolled()
  {
  }
}
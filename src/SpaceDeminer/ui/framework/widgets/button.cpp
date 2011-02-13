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

#include "./button.hpp"

namespace Framework
{
  Button::Button()
  {
    _relief_type  = RELIEF_ALWAYS;
    _state  = DRAW_PARAM_RELIEF_NORMAL;
    child_dist.set(3);

    set_name("button");

    can_process_mouse_move_events = true;
  }

  Button::Button(Glib::ustring& label, bool use_memnotic)
  {
    _relief_type  = RELIEF_ALWAYS;
    _state  = DRAW_PARAM_RELIEF_NORMAL;
    child_dist.set(3);

    set_name("button");

    set_label(label, use_memnotic);

    can_process_mouse_move_events = true;
  }

  Button::~Button()throw()
  {
  }

  void Button::set_theme(const Glib::RefPtr<Theme>& theme)
  {
    Bin::set_theme(theme);
    if(theme)
      theme->get_metrics("child-dist/Button/Button/"+get_name(), child_dist);
  }

  void Button::set_relief(ReliefType rt)
  {
    _relief_type  = rt;

    invalidate();
  }

  void Button::on_size_request(int& w, int& h)
  {
    Bin::on_size_request(w, h);

    w += child_dist.x1+child_dist.x2;
    h += child_dist.y1+child_dist.y2;
  }

  void Button::v_rearrange_children()
  {
    if(!get_child())
      return;

    set_child_allocation(*get_child(), get_border_width()+child_dist.x1, get_border_width()+child_dist.y1, MAX(0,get_width()-get_border_width()*2-child_dist.x1-child_dist.x2), MAX(0,get_height()-get_border_width()*2-child_dist.y1-child_dist.y2));
  }

  void Button::on_expose(EventExpose& paint_tool)
  {
    if(get_relief()!=RELIEF_NEVER)
    {
      paint_tool.draw_widget_back("Button/Button", _state, *this);
    }

    if(get_state()==DRAW_PARAM_RELIEF_PUSHED)
      paint_tool.push_offset(1, 1);

    Container::on_expose(paint_tool);

    if(get_state()==DRAW_PARAM_RELIEF_PUSHED)
      paint_tool.pop_offset();
  }

  bool Button::on_mouse_move(MouseEvent& mouse_event)
  {
    if(Container::on_mouse_move(mouse_event))
      return true;

    return true;
  }

  bool Button::on_mouse_enter(MouseEvent& mouse_event)
  {
    ParentClass::on_mouse_enter(mouse_event);

    _state  = DRAW_PARAM_RELIEF_MOUSEOVER;

    invalidate();

    return true;
  }
  bool Button::on_mouse_leave(MouseEvent& mouse_event)
  {
    ParentClass::on_mouse_leave(mouse_event);

    _state  = DRAW_PARAM_RELIEF_NORMAL;

    invalidate();

    return true;
  }

  bool Button::on_button_press(MouseButtonEvent& mouse_event)
  {
    ParentClass::on_button_press(mouse_event);

    _state  = DRAW_PARAM_RELIEF_PUSHED;

    invalidate();

    return true;
  }

  bool Button::on_button_release(MouseButtonEvent& mouse_event)
  {
    ParentClass::on_button_release(mouse_event);

    _state  = DRAW_PARAM_RELIEF_MOUSEOVER;

    if(mouse_event.x >= 0 &&
       mouse_event.y >= 0 &&
       mouse_event.x <  get_width() &&
       mouse_event.y <  get_height())
      on_clicked();

    invalidate();
    return true;
  }

  //--------

  ToggleButton::ToggleButton()
  {
    set_name("togglebutton");
  }

  void ToggleButton::set_theme(const Glib::RefPtr<Theme>& theme)
  {
    Bin::set_theme(theme);
    theme->get_metrics("child-dist/Button/ToggleButton/"+get_name(), child_dist);
  }
}
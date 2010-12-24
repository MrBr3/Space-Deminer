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

#include "./../framework.hpp"

namespace Framework
{
  Widget::MouseButtonEvent::MouseButtonEvent(int btn, guint pressed_times)
  {
    this->btn = btn;
    this->pressed_times  = pressed_times;
  }

//--------------------------------

  Widget::Widget() : _allocation(0, 0, 0, 0)
  {
    _is_managed = false;
    _visible = false;
    _sensitive  = true;
    _use_show_all_children  = true;
    _memnotic = 0;
    _parent = nullptr;

    _size_request_width = 0;
    _size_request_height = 0;
    _custom_size_request_width  = false;
    _custom_size_request_height = false;

    can_process_mouse_move_events = false;
    can_process_mouse_drag_events = false;
  }

  Widget::~Widget()throw()
  {
    remove_any_focus();

    if(get_parent())
      get_parent()->remove(*this);
  }

  void Widget::set_theme(const Glib::RefPtr<Theme>& theme)
  {
    _theme=theme;
  }

  void Widget::set_window_manager(WindowManager* wm)
  {
    Container* container  = dynamic_cast<Container*>(this);

    _window_manager = wm;

    if(container)
    {
      for(std::list<Widget*>::const_iterator c_iter = container->get_children().begin(); c_iter != container->get_children().end(); ++c_iter)
      {
        g_assert(*c_iter);
        const_cast<Widget*>(*c_iter)->set_window_manager(wm);
      }
    }
  }

  void Widget::remove_any_focus()
  {
    if(!_window_manager)
      return;

    _window_manager->remove_any_focus(this);

    invalidate();
  }

  void Widget::set_sensitive(bool s)
  {
    if(s==_sensitive)
      return;
    remove_any_focus();
    _sensitive  = s;
    invalidate();
  }

  void Widget::show(bool show)
  {
    if(_visible==show)
      return;

    _visible  = show;

    if(get_parent())
    {
      get_parent()->on_child_visibility_changed(*this);
    }

    remove_any_focus();

    on_visibility_changed();
  }

  void Widget::set_name(const Glib::ustring& name)
  {
    _name = name;

    recalc_size_request();
    invalidate();
  }

  /*void Widget::on_realize()
  {
  }

  void Widget::on_unrealize()
  {
  }*/

  void Widget::on_expose(EventExpose& paint_tool)
  {
  }

  void Widget::on_debug_draw(EventExpose& paint_tool)const
  {
    Real col_r  = 1.f, col_g=1.f, col_b=1.f;

    paint_tool.draw_rect_border(get_allocation(), col_r, col_g, col_b);
  }

  void Widget::invalidate(Gdk::Region& region)
  {
    on_invalidate(region);

    if(!get_parent())
      return;
    region.offset(get_allocation().get_x(), get_allocation().get_y());
    get_parent()->on_children_invalidate(*this, region);
  }

  Widget* Widget::find_widget(MouseEvent& event)
  {
    if(!is_visible() || !get_sensitive() || !is_in_rectangle(event.x, event.y, get_allocation()))
      return nullptr;

    Container* self_container = dynamic_cast<Container*>(this);

    Widget* w = nullptr;

    if(self_container)
    {
      event.x -= get_allocation().get_x();
      event.y -= get_allocation().get_y();

      for(std::list<Widget*>::const_reverse_iterator child_iter = self_container->get_children().rbegin(); !w && child_iter != self_container->get_children().rend(); ++child_iter)
      {
        w = const_cast<Widget*>(*child_iter)->find_widget(event);
      }

      event.x += get_allocation().get_x();
      event.y += get_allocation().get_y();
    }

    return w ? w : this;
  }

  void Widget::recalc_size_request()
  {
    if(is_custom_size_request())
      return;

    int old_sr_w=_size_request_width, old_sr_h=_size_request_height;

    int w=0, h=0;

    on_size_request(w, h);

    if(!_custom_size_request_width && _size_request_width!=w)
      _size_request_width  = w;
    if(!_custom_size_request_height && _size_request_height!=h)
      _size_request_height  = h;

    _size_request_width  = MAX(0, _size_request_width);
    _size_request_height = MAX(0, _size_request_height);

    if((old_sr_w!=_size_request_width || old_sr_h!=_custom_size_request_height) && get_parent())
    {
      get_parent()->on_child_size_request_changed(*this);
      invalidate();
    }
  }

  void Widget::set_size_request(int width, int height)
  {
    bool recalc = false;
    bool something_changed=false;

    if(width<0 && _custom_size_request_width)
    {
      _custom_size_request_width  = false;
      recalc  = true;
    }
    if(height<0 && _custom_size_request_height)
    {
      _custom_size_request_height  = false;
      recalc  = true;
    }

    if(width>=0 && _size_request_width!=width)
    {
      _custom_size_request_width  = true;
      _size_request_width = width;
      something_changed = true;
    }
    if(height>=0 && _size_request_height!=height)
    {
      _custom_size_request_height  = true;
      _size_request_height = height;
      something_changed = true;
    }

    if(recalc)
      recalc_size_request();
    else if(something_changed && get_parent())
    {
      get_parent()->on_child_size_request_changed(*this);
      invalidate();
    }
  }
}
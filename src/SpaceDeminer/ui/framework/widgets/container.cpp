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

#include "./../window.hpp"

namespace Framework
{
  Container::Container()
  {
    _border_width = 0;
    _children_size_request_width  = 0;
    _children_size_request_height  = 0;
    set_name("container");
  }

  Container::~Container()throw()
  {
    for(std::list<Widget*>::iterator iter = _children.begin(); iter!=_children.end(); iter=_children.erase(iter))
    {
      Widget& child = **iter;

      g_assert(child.get_parent()==this);

      child._parent = nullptr;
    }
  }

  void Container::set_theme(const Glib::RefPtr<Theme>& theme)
  {
    Widget::set_theme(theme);

    for(std::list<Widget*>::iterator iter = _children.begin(); iter!=_children.end(); ++iter)
    {
      Widget& child = **iter;

      g_assert(&child);

      child.set_theme(theme);
      child.recalc_size_request();
    }
  }

  void Container::on_size_request(int& w, int& h)
  {
    Widget::on_size_request(w, h);

    w = get_border_width()*2;
    h = get_border_width()*2;
  }

  void Container::rearrange_children()
  {
    if(!_children.size())
      return;

    if(get_width() < get_size_request_width() || get_height() < get_size_request_height())  // TODO Check, whether this is ...
    {
      Allocation& allocation_ = const_cast<Allocation&>(get_allocation());

      int _width  = allocation_.get_width();
      int _height = allocation_.get_height();

       allocation_.set_width (MAX(_width,  get_size_request_width ()));
       allocation_.set_height(MAX(_height, get_size_request_height()));

      v_rearrange_children();

       allocation_.set_width(_width);
       allocation_.set_height(_height);
    }else// ... needed
      v_rearrange_children();
  }

  void Container::v_rearrange_children()
  {
    for(std::list<Widget*>::iterator iter = _children.begin(); iter!=_children.end(); ++iter)
    {
      Widget& child = **iter;

      set_child_allocation(child, 0, 0, 0, 0);
    }
  }

  void Container::on_size_allocate()
  {
    rearrange_children();
  }

  void Container::on_child_size_request_changed(const Widget& child)
  {
    g_assert(this);

    Gdk::Rectangle alloc  = get_allocation();
    recalc_size_request();
    if(alloc==get_allocation())
    {
      rearrange_children();
      invalidate();

      Container* c_child  = dynamic_cast<Container*>(const_cast<Widget*>(&child));
      if(c_child)
        c_child->rearrange_children();
    }
  }

  void Container::on_child_visibility_changed(Widget& child)
  {
    Gdk::Rectangle alloc  = get_allocation();
    recalc_size_request();
    if(alloc==get_allocation())
    {
      rearrange_children();
      invalidate();
    }
  }

  void Container::set_child_allocation(Widget& child, const Gdk::Rectangle& new_alloc)
  {
    if(!&child)
      throw std::invalid_argument("child");
    if(child.get_parent()!=this)
      throw std::invalid_argument("child is not a child");

    if(new_alloc.get_width()<0)
      throw std::invalid_argument("new_alloc.get_width() should be >=0");
    if(new_alloc.get_height()<0)
      throw std::invalid_argument("new_alloc.get_height() should be >=0");

    Gdk::Rectangle& old_alloc = const_cast<Gdk::Rectangle&>(child.get_allocation());

    if(old_alloc==new_alloc)
      return;

    old_alloc = new_alloc;

    child.on_size_allocate();
  }

  void Container::on_expose(EventExpose& paint_tool)
  {
    //paint_tool.push_scissor(get_allocation()); // uncomment if you want to allo draw outside the widgets area

    for(std::list<Widget*>::iterator iter = _children.begin(); iter!=_children.end(); ++iter)
    {
      Widget& child = **iter;

      //std::cout<<"child.is_visible() "<<child.is_visible()<<" paint_tool.is_in_region(child.get_allocation()) "<<paint_tool.is_in_region(child.get_allocation())<<"\n";
      //std::cout<<"child.get_allocation() "<<child.get_allocation().get_x()<<", "<<child.get_allocation().get_y()<<", "<<child.get_allocation().get_width()<<", "<<child.get_allocation().get_height()<<"\n\n";

      if(child.is_visible() /*&& paint_tool.is_in_region(child.get_allocation())*/)
      {
        paint_tool.push_scissor(child.get_allocation());
        paint_tool.push_offset(child.get_allocation().get_x(), child.get_allocation().get_y());

        child.on_expose(paint_tool);

        paint_tool.pop_offset();
        paint_tool.pop_scissor();
      }
    }
    //paint_tool.pop_scissor();
  }

  void Container::draw_childrens_debug(EventExpose& paint_tool)const
  {
    paint_tool.push_offset(get_allocation().get_x(), get_allocation().get_y());
    for(std::list<Widget*>::const_iterator iter = _children.begin(); iter!=_children.end(); ++iter)
    {
      const Widget& child = **iter;

      //std::cout<<"child.is_visible() "<<child.is_visible()<<" paint_tool.is_in_region(child.get_allocation()) "<<paint_tool.is_in_region(child.get_allocation())<<"\n";
      //std::cout<<"child.get_allocation() "<<child.get_allocation().get_x()<<", "<<child.get_allocation().get_y()<<", "<<child.get_allocation().get_width()<<", "<<child.get_allocation().get_height()<<"\n\n";

      if(child.is_visible() /*&& paint_tool.is_in_region(child.get_allocation())*/)
      {
        paint_tool.push_scissor(child.get_allocation());

        child.on_debug_draw(paint_tool);

        paint_tool.pop_scissor();
      }
    }
    paint_tool.pop_offset();
  }

  void Container::on_debug_draw(EventExpose& paint_tool)const
  {
    draw_childrens_debug(paint_tool);
    Widget::on_debug_draw(paint_tool);
  }

  void Container::validate_child_(Widget& child)
  {
    if(&child == this || !&child)
      throw IllegalParam();
    if(child.get_parent())
      throw WidgetHasAlreadyParent();
  }

  void Container::add_child_(Widget& child)
  {
    child._parent = this;

    _children.push_back(&child);

    if(get_theme() && !child.get_theme())
      child.set_theme(get_theme());

    child._window_manager = _window_manager;

    child.remove_any_focus();

    if(child.is_visible())
    {
      on_child_visibility_changed(child);
    }
  }

  void Container::remove_all()throw()
  {
    Widget* old_widget = nullptr;
    for(std::list<Widget*>::iterator iter=_children.begin(); iter!=_children.end(); iter=_children.begin())
    {
      g_assert(*iter);
      g_assert(*iter!=old_widget);
      old_widget  = *iter;

      remove(**iter);
    }
  }

  void Container::remove(Widget& child)
  {
    std::list<Widget*>::iterator iter = _children.begin();
    for(; iter!=_children.end(); ++iter)
    {
      if(*iter==&child)
        break;
    }

    if(iter==_children.end())
      throw NotAChild();

    g_assert(child.get_parent()==this);

    _children.erase(iter);

    child._parent = nullptr;
    child._window_manager = nullptr;

    if(get_theme() == child.get_theme())
      child.set_theme(Glib::RefPtr<Theme>());

    child.remove_any_focus();

    if(child.is_visible())
    {
      on_child_visibility_changed(child);
    }
  }

  void Container::set_border_width(int border_width)
  {
    _border_width = border_width;

    recalc_size_request();
    invalidate();
  }

  void Container::show_all_children(bool recursice, bool show)
  {
    for(std::list<Widget*>::iterator iter = _children.begin(); iter!=_children.end(); ++iter)
    {
      Widget& child = **iter;

      g_assert(&child);

      if(!child.get_use_show_all_children())
        continue;

      child.show(show);

      Container* container_child  = dynamic_cast<Container*>(&child);

      if(recursice && container_child)
      {
        container_child->show_all_children(true, show);
      }
    }
  }

  void Container::on_children_invalidate(const Widget& child, Gdk::Region& region)
  {
    region.offset(get_allocation().get_x(), get_allocation().get_y());

    on_invalidate(region);

    if(!get_parent())
      return;

    get_parent()->on_children_invalidate(*this, region);
  }

  Widget* Container::find_mouse_widget(const Widget::MouseEvent& mouse_event)
  {
    for(std::list<Widget*>::iterator iter = _children.begin(); iter!=_children.end(); ++iter)
    {
      Widget& child = **iter;

      g_assert(&child);

      if(is_in_rectangle(mouse_event.x, mouse_event.y, child.get_allocation()))
        return &child;
    }

    return nullptr;
  }

}
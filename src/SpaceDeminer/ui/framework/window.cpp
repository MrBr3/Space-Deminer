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

#include "./window.hpp"
#include "./../menu.hpp"
#include "./../main-window.hpp"

namespace Framework
{
  Window::Window()
  {
    _window_manager = nullptr;
    set_name("window");
  }

  Window::~Window()throw()
  {
    on_visibility_changed();

    if(is_registered())
    {
      Gdk::Region region(get_allocation());
      get_window_manager()->invalidate(region);

#ifdef NDEBUG
      int i=0;
#endif
      WindowManager::WindowMap& _windows  = get_window_manager()->_windows;

      for(WindowManager::WindowMap::iterator iter=_windows.begin(); iter!=_windows.end();)
      {
        if(iter->second==this)
        {
#ifdef NDEBUG
          g_assert(i==0);
          ++i;
#endif

          WindowManager::WindowMap::iterator c  = iter;
          ++iter;

          _windows.erase(c);
        }else
        {
          ++iter;
        }
      }

#ifdef NDEBUG
      g_assert(i==1);
#endif
    }
  }

  void Window::set_theme(const Glib::RefPtr<Theme>& theme)
  {
    Bin::set_theme(theme);
    recalc_size_request();
  }

  void Window::on_visibility_changed()
  {
    Bin::on_visibility_changed();

    Gdk::Region region(get_allocation());
    get_window_manager()->invalidate(region);
  }

  void Window::on_invalidate(Gdk::Region& region)
  {
    if(get_window_manager())
      get_window_manager()->invalidate(region);
  }

  void Window::on_size_request(int& w, int& h)
  {
    Container::on_size_request(w, h);

    if(get_child())
    {
      w+=get_child()->get_size_request_width();
      h+=get_child()->get_size_request_height();
    }
  }

  void Window::v_rearrange_children()
  {
    g_assert(get_child());

    set_child_allocation(*get_child(),
      get_border_width(),
      get_border_width(),
        get_width()-(get_border_width()*2),
        get_height()-(get_border_width()*2));
  }

  void Window::set_size(int w, int h)
  {
    get_allocation().set_width(w);
    get_allocation().set_height(h);

    on_size_allocate();
  }

  void Window::on_register_window(int layer)
  {
    invalidate();
  }

  //----FullscreenWindow----

  void FullscreenWindow::on_register_window(int layer)
  {
    set_name("fullscreen-window");

    Window::on_register_window(layer);

    get_window_manager()->signal_size_changed().connect(sigc::mem_fun(*this, &Window::set_size));

    Window::set_size(MAX(get_size_request_width(), get_window_manager()->get_width()), MAX(get_size_request_height(), get_window_manager()->get_height()));
  }

  FullscreenWindow::~FullscreenWindow()throw()
  {
  }

  //----WindowManager----

  WindowManager::WindowManager()
  {
    _width  = 0;
    _height = 0;

    _debug_ui  = DEBUG_UI_NONE;

    full_area_invalid  = true;

    init_mouse_state_machine();
  }

  WindowManager::~WindowManager()throw()
  {
    for(WindowManager::WindowMap::iterator iter=_windows.begin(); iter!=_windows.end(); )
    {
      g_assert(iter->second);
      if(iter->second->get_theme()==get_theme())
        iter->second->set_theme(Glib::RefPtr<Theme>());
      iter->second->_window_manager = nullptr;

      WindowManager::WindowMap::iterator c  = iter;
      iter++;

      _windows.erase(c);
    }
  }

  void WindowManager::set_theme(const Glib::RefPtr<Theme>& theme)
  {
    g_assert(this);
    _theme  = theme;
    for(WindowManager::WindowMap::iterator iter=_windows.begin(); iter!=_windows.end(); ++iter)
    {
      iter->second->set_theme(theme);
    }
  }

  void WindowManager::register_window(int layer, Window& w)
  {
    g_assert(this);
    if(w.is_registered())
      throw WindowIsAlreadyRegistered();

    w._window_manager = this;

    if(_windows.find(layer)!=_windows.end())
      throw LayerAlreadyInUse();

    _windows[layer] = &w;

    if(get_theme() && !w.get_theme())
      w.set_theme(get_theme());

    w.set_window_manager(this);

    w.on_register_window(layer);
  }

  void WindowManager::expose()
  {
    g_assert(this);
    if(!get_theme())
      set_theme(DummyTheme::create());
    g_assert(get_theme());

    try
    {
      Glib::RefPtr<PaintTool> paint_tool_  = Engine::get_singleton()->create_paint_tool(invalid_region, *this);
      PaintTool& paint_tool = *paint_tool_.operator->();

      int isd = paint_tool.get_scissor_stack_depth();
      int iod = paint_tool.get_offset_stack_depth();

      for(WindowManager::WindowMap::iterator iter=_windows.begin(); iter!=_windows.end(); ++iter)
      {
        g_assert(iter->second);

        Window& window  = *iter->second;

        g_assert(window.is_registered());

        paint_tool.push_scissor(window.get_allocation());
        paint_tool.push_offset(window.get_allocation().get_x(), window.get_allocation().get_y());
        if(window.is_visible() /*&& paint_tool.is_in_region(window.get_allocation())*/)
        {
          window.on_expose(paint_tool);
        }
        paint_tool.pop_offset();
        paint_tool.pop_scissor();

        if(paint_tool.get_scissor_stack_depth()!=isd)
          throw std::out_of_range("paint_tool push_scissor() hasn't been called as often as pop_scissor()!");
        if(paint_tool.get_offset_stack_depth()!=iod)
          throw std::out_of_range("paint_tool push_offset() hasn't been called as often as pop_offset()!");

        if(get_debug_ui()==DEBUG_UI_WIDGET_NAMES)
        {
          debug_ui_widget_names(paint_tool, window);
        }
      }

      invalid_region  = Gdk::Region();
      full_area_invalid = false;
    }catch(const Glib::Exception& e)
    {
      std::cout<<"**WindowManager::expose** an Glib::Exception has been detected: '"<<e.what()<<"'\n";
      throw;
    }catch(const std::exception& e)
    {
      std::cout<<"**WindowManager::expose** an std::exception has been detected: '"<<e.what()<<"'\n";
      throw;
    }catch(...)
    {
      std::cout<<"**WindowManager::expose** an unkown exception has been detected!\n";
      throw;
    }
  }

  void WindowManager::debug_ui_widget_names(PaintTool& paint_tool, const Widget& w)
  {
    g_assert(this);/*
    paint_tool.draw_rect_border(w.get_allocation(), 1.f, 1.f, 1.f);
    paint_tool.push_offset(w.get_allocation().get_x(), w.get_allocation().get_y());

    const Container* c  = dynamic_cast<const Container*>(&w);

    if(c)
    {
      for(std::list<Widget*>::const_iterator iter = c->get_children().begin(); iter!=c->get_children().end(); ++iter)
      {
        debug_ui_widget_names(paint_tool, **iter);
      }
    }

    paint_tool.pop_offset();*/
    w.on_debug_draw(paint_tool);
  }

  void WindowManager::invalidate()
  {
    g_assert(this);
    full_area_invalid = true;
    invalid_region  = Gdk::Region(Gdk::Rectangle(0, 0, _width, _height));

    {
      Glib::RefPtr<Gdk::Window> window = MainWindow::get_singleton()->gl_drawing_area.get_window();
      if(window)
        window->invalidate(true);
    }
  }

  void WindowManager::invalidate(const Gdk::Region& region)
  {
    g_assert(this);
    if(full_area_invalid)
      return;

    invalid_region  = Gdk::Region();

    invalid_region.union_(region);

    {
      Glib::RefPtr<Gdk::Window> window = MainWindow::get_singleton()->gl_drawing_area.get_window();
      if(window)
        window->invalidate_region(region, true);
    }
  }

  //----MouseEvents--------

  namespace Window_Private
  {
    typedef WindowManager::MouseStateHandler MouseStateHandler;

    class MouseState_outside : public MouseStateHandler
    {
    public:
      bool on_mouse_move(Widget::MouseEvent& mouse_event)
      {
        g_assert(get_state_machine());
        Widget* w = get_state_machine()->get_widget(mouse_event);

        if(w)
        {
          g_assert(get_state_machine());
          get_state_machine()->correct_coordinates(w, mouse_event);

          std::cout<<"w->get_name() "<<w->get_name().data()<<"\n";

          bool b  = w->on_mouse_enter(mouse_event);
          if(w->on_mouse_move(mouse_event))
            b = true;

          get_state_machine()->activate_state(WindowManager::MOUSE_STATE_OVER);

          return b;
        }
        return false;
      }
      bool on_mouse_enter(Widget::MouseEvent& mouse_event)
      {
        return false;
      }
      bool on_mouse_leave(Widget::MouseEvent& mouse_event)
      {
        return false;
      }
      bool on_button_press(Widget::MouseButtonEvent& mouse_event)
      {
        return false;
      }
      bool on_button_release(Widget::MouseButtonEvent& mouse_event)
      {
        return false;
      }
      void remove_any_focus(Widget* w)
      {
      }

      void on_activate()
      {
        g_assert(get_state_machine());
        get_state_machine()->_mouse_focus  = nullptr;
        get_state_machine()->_mouse_over  = nullptr;
      }

      void on_deactivate()
      {
      }

      static Glib::RefPtr<MouseState_outside> create(){return Glib::RefPtr<MouseState_outside>(new MouseState_outside);}

      MouseState_outside()
      {
      }

      ~MouseState_outside()throw()
      {
      }
    };

    class MouseState_over : public MouseStateHandler
    {
    public:
      bool on_mouse_move(Widget::MouseEvent& mouse_event)
      {
        return false;
      }
      bool on_mouse_enter(Widget::MouseEvent& mouse_event)
      {
        return false;
      }
      bool on_mouse_leave(Widget::MouseEvent& mouse_event)
      {
        return false;
      }
      bool on_button_press(Widget::MouseButtonEvent& mouse_event)
      {
        return false;
      }
      bool on_button_release(Widget::MouseButtonEvent& mouse_event)
      {
        return false;
      }
      void remove_any_focus(Widget* w)
      {
      }

      void on_activate()
      {
      }

      void on_deactivate()
      {
      }

      static Glib::RefPtr<MouseState_over> create(){return Glib::RefPtr<MouseState_over>(new MouseState_over);}

      MouseState_over()
      {
      }

      ~MouseState_over()throw()
      {
      }
    };
  }

  using namespace Window_Private;

  Widget* WindowManager::MouseStateMachine::get_widget(Widget::MouseEvent& mouse_event)
  {
    g_assert(this);
    Window* window  = get_window(mouse_event);

    if(!window)
      return nullptr;

    return window->find_widget(mouse_event);
  }

  void WindowManager::MouseStateMachine::correct_coordinates(Widget* w, Widget::MouseEvent& mouse_event)
  {
    Container* parent = w->get_parent();

    mouse_event.x -= w->get_allocation().get_x();
    mouse_event.y -= w->get_allocation().get_y();

    while(parent)
    {
      mouse_event.x -= parent->get_allocation().get_x();
      mouse_event.y -= parent->get_allocation().get_y();

      parent  = parent->get_parent();
    }
  }

  ObsLink<Window> WindowManager::get_window(int x, int y, bool back_to_front)
  {
    if(!back_to_front)
    {
      for(WindowMap::iterator w_iter=_windows.begin(); w_iter!=_windows.end(); ++w_iter)
      {
        g_assert(w_iter->second);
        if(!w_iter->second || !w_iter->second->is_visible() || !w_iter->second->get_sensitive())
          continue;
        if(is_in_rectangle(x, y, w_iter->second->get_allocation()))
          return w_iter->second;
      }
    }else
    {
      for(WindowMap::reverse_iterator w_iter=_windows.rbegin(); w_iter!=_windows.rend(); ++w_iter)
      {
        g_assert(w_iter->second);
        if(!w_iter->second || !w_iter->second->is_visible() || !w_iter->second->get_sensitive())
          continue;
        if(is_in_rectangle(x, y, w_iter->second->get_allocation()))
          return w_iter->second;
      }
    }

    return nullptr;
  }

  void WindowManager::init_mouse_state_machine()
  {
    mouse_state_machine._window_manager = this;
    mouse_state_machine.add_state(MOUSE_STATE_OUTSIDE, MouseState_outside::create());
    mouse_state_machine.add_state(MOUSE_STATE_OVER, MouseState_over::create());
    mouse_state_machine.activate_state(MOUSE_STATE_OUTSIDE);
  }

  void WindowManager::MouseStateMachine::remove_any_focus(Widget* w)
  {
    if(w==_mouse_over || w==_mouse_focus)
    {
      _mouse_over  = nullptr;
      _mouse_focus  = nullptr;
      activate_state(MOUSE_STATE_OUTSIDE);
    }

    g_assert(get_active_state());
    if(!get_active_state())
      return;
    get_active_state()->remove_any_focus(w);
  }

  bool WindowManager::on_mouse_move(Widget::MouseEvent& mouse_event)
  {
    g_assert(mouse_state_machine.get_active_state());
    if(!mouse_state_machine.get_active_state())
      return false;
    return mouse_state_machine.get_active_state()->on_mouse_move(mouse_event);
  }

  bool WindowManager::on_mouse_enter(Widget::MouseEvent& mouse_event)
  {
    g_assert(mouse_state_machine.get_active_state());
    if(!mouse_state_machine.get_active_state())
      return false;
    return mouse_state_machine.get_active_state()->on_mouse_enter(mouse_event);
  }

  bool WindowManager::on_mouse_leave(Widget::MouseEvent& mouse_event)
  {
    g_assert(mouse_state_machine.get_active_state());
    if(!mouse_state_machine.get_active_state())
      return false;
    return mouse_state_machine.get_active_state()->on_mouse_leave(mouse_event);
  }

  bool WindowManager::on_button_press(Widget::MouseButtonEvent& mouse_event)
  {
    g_assert(mouse_state_machine.get_active_state());
    if(!mouse_state_machine.get_active_state())
      return false;
    return mouse_state_machine.get_active_state()->on_button_press(mouse_event);
  }

  bool WindowManager::on_button_release(Widget::MouseButtonEvent& mouse_event)
  {
    g_assert(mouse_state_machine.get_active_state());
    if(!mouse_state_machine.get_active_state())
      return false;
    return mouse_state_machine.get_active_state()->on_button_release(mouse_event);
  }

  bool WindowManager::handle_gtk_motion_notify_event(GdkEventMotion* e)
  {
    g_assert(e);

    Widget::MouseEvent mouse_event;

    mouse_event.x  = e->x;
    mouse_event.y  = e->y;

    return on_mouse_move(mouse_event);
  }

  bool WindowManager::handle_gtk_leave_notify_event(GdkEventCrossing* e)
  {
    g_assert(this);
    g_assert(e);

    Widget::MouseEvent mouse_event;

    mouse_event.x  = e->x;
    mouse_event.y  = e->y;

    return on_mouse_leave(mouse_event);
  }

  bool WindowManager::handle_gtk_enter_notify_event(GdkEventCrossing* e)
  {
    g_assert(e);

    Widget::MouseEvent mouse_event;

    mouse_event.x  = e->x;
    mouse_event.y  = e->y;

    return on_mouse_enter(mouse_event);
  }
}
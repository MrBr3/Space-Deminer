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

#ifndef _SPACEDEMINER_UI_FRAMEWORK_UI_H_
#define _SPACEDEMINER_UI_FRAMEWORK_UI_H_

#include "./widgets/bin.hpp"

namespace Framework
{
  class WindowManager;
  class Window : public Bin
  {
  public:
    friend class WindowManager;

  public:
    virtual void on_register_window(int layer);

    void set_size(int w, int h);

    WindowManager* get_window_manager(){return _window_manager;}
    const WindowManager* get_window_manager()const{return _window_manager;}

    Widget::Allocation& get_allocation()throw(){g_assert(this);return const_cast<Widget::Allocation&>(Widget::get_allocation());}
    void on_size_request(int& w, int& h);
    void on_invalidate(Gdk::Region& region);

    void on_visibility_changed();

    void set_theme(const Glib::RefPtr<Theme>& theme);

    Window();
    ~Window()throw();

    void v_rearrange_children();

    bool is_registered()const throw(){g_assert(this);return _window_manager;}

  private:
    WindowManager* _window_manager;
  };

  class FullscreenWindow : public Window
  {
  public:
    void on_register_window(int layer);

    ~FullscreenWindow()throw();
  };

  class WindowManager : public Refable
  {
    friend Window::~Window()throw();
  public:
    class WindowIsAlreadyRegistered : public Glib::Exception
    {
    public:
      Glib::ustring what()const{return _("window is alredy registered");}

      ~WindowIsAlreadyRegistered()throw(){}
    };
    class LayerAlreadyInUse : public Glib::Exception
    {
    public:
      Glib::ustring what()const{return _("the layer is alredy in use");}

      ~LayerAlreadyInUse()throw(){}
    };

    WindowManager();
    ~WindowManager()throw();

    int get_width() const throw(){return _width;}
    int get_height()const throw(){return _height;}

    void set_size(int w, int h)
    {
      _width  = w;
      _height = h;

      invalidate();
      signal_size_changed().emit(w, h);
    }

    const Glib::RefPtr<Theme>& get_theme(){return _theme;}
    Glib::RefPtr<const Theme> get_theme()const{return _theme;}
    void set_theme(const Glib::RefPtr<Theme>& theme);

    void register_window(int layer, Window& w);

  public:
    void expose();

    void invalidate();
    void invalidate(const Gdk::Region& region);

    Gdk::Region invalid_region;
    bool full_area_invalid;

    const sigc::signal<void, int, int>& signal_size_changed()const{return _signal_size_changed;}
    sigc::signal<void, int, int>& signal_size_changed(){return _signal_size_changed;}

    enum DebugUI
    {
      DEBUG_UI_NONE = 0,
      DEBUG_UI_WIDGET_NAMES,
      DEBUG_UI_N
    };

    void set_debug_ui(DebugUI dui){_debug_ui=dui;invalidate();}
    DebugUI get_debug_ui()const{return _debug_ui;}

    bool on_mouse_move(Widget::MouseEvent& mouse_event);
    bool on_mouse_enter(Widget::MouseEvent& mouse_event);
    bool on_mouse_leave(Widget::MouseEvent& mouse_event);
    bool on_button_press(Widget::MouseButtonEvent& mouse_event);
    bool on_button_release(Widget::MouseButtonEvent& mouse_event);

    /** \brief Removes any focus from the given widget.
     *
     * After calling this function, the widget will have no focus anymore: Neighter mouse nor keyfocus.
     * This Function gets called, when the widgets parent, visibility or sensibililty has changed.
     * */
    void remove_any_focus(Widget* w)
    {
      mouse_state_machine.remove_any_focus(w);
    }

    bool handle_gtk_motion_notify_event(GdkEventMotion* e);
    bool handle_gtk_leave_notify_event(GdkEventCrossing* e);
    bool handle_gtk_enter_notify_event(GdkEventCrossing* e);

  private:
    typedef std::map<int, ObsLink<Window> > WindowMap;
    WindowMap _windows;

    //---- handling input events stuff---
  public:
    enum MouseState
    {
      MOUSE_STATE_OUTSIDE = 0,
      MOUSE_STATE_OVER,
    };

    ObsLink<Window> get_window(int x, int y, bool back_to_front=false);

    /** \brief Initialies the state machine.
     * */
    void init_mouse_state_machine();

    class MouseStateHandler;
    class MouseStateMachine : public StateMachine<MouseStateMachine, MouseStateHandler, MouseState>
    {
    public:
      ObsLink<Widget> _mouse_focus, _mouse_over;
      ObsLink<WindowManager> _window_manager;

      ObsLink<Window> get_window(int x, int y){g_assert(_window_manager);return _window_manager->get_window(x, y);}
      ObsLink<Window> get_window(const Widget::MouseEvent& mouse_event){g_assert(_window_manager);return _window_manager->get_window(mouse_event.x, mouse_event.y, true);}
      Widget* get_widget(Widget::MouseEvent& mouse_event);

      /** \brief Calculates the x/y coordinates of mouse_event to be relative to w.
       *
       * \param w the widget, the coordinates will be relative to
       *
       * \param mouse_event the mouse_event, which's coordinates will be be corrected
       * */
      void correct_coordinates(Widget* w, Widget::MouseEvent& mouse_event);

      MouseStateMachine()
      {
        _mouse_focus  = _mouse_over = nullptr;
      }

      ~MouseStateMachine()throw()
      {
      }

      void remove_any_focus(Widget* w);
    }mouse_state_machine;

    class MouseStateHandler : public MouseStateMachine::State
    {
    public:
      /** \brief Gets the widget, the mouse is pointing to and is able to process mouse-events
       *
       * After calling this function, the coordinates of the mouse_event will be relative to the widget returned.
       *
       * \param mouse_event a reference to a mouse event, defining the mouse pos, (relative to the WindowManager)
       * \param move true if the widget must be able to process on_mouse_move callbacks in order to be found
       * <!--\param drag - true if the widget must be able to process on_mouse_drag callbacks in order to be found-->
       *
       * \return a pointer to the widget - can be nullptr if no widget could be found
       * */
      Widget* find_widget_accepting_mouse(Widget::MouseEvent& mouse_event, bool move/*, bool drag*/);

      /**
       * \note The default implementation just searches the window, the mouse points in and sends
       * */
      virtual bool on_mouse_move(Widget::MouseEvent& mouse_event)=0;
      virtual bool on_mouse_enter(Widget::MouseEvent& mouse_event)=0;
      virtual bool on_mouse_leave(Widget::MouseEvent& mouse_event)=0;
      virtual bool on_button_press(Widget::MouseButtonEvent& mouse_event)=0;
      virtual bool on_button_release(Widget::MouseButtonEvent& mouse_event)=0;
      virtual void remove_any_focus(Widget* w)=0;

      ~MouseStateHandler()throw()
      {
      }
    };
    //----
    private:

    Glib::RefPtr<Theme> _theme;

    DebugUI _debug_ui;

    int _width;
    int _height;

    sigc::signal<void, int, int> _signal_size_changed;

    void debug_ui_widget_names(PaintTool& paint_tool, const Widget& w);
  };
}

#endif
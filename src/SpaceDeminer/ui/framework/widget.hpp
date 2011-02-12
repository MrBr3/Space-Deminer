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

#ifndef _SPACEDEMINER_UI_FRAMEWORK_WIDGET_H_
#define _SPACEDEMINER_UI_FRAMEWORK_WIDGET_H_

#include "./engine.hpp"

namespace Framework
{
  class Theme;
  class Container;
  class WindowManager;
  class Widget : public Refable
  {
    Widget(const Widget&);

  public:
    class NotRealized : public Glib::Exception
    {
    public:
      Glib::ustring what()const{return _("widget is not realized");}

      ~NotRealized()throw(){}
    };

  //----Stuff----
  public:
    void show(bool show=true);
    void hide(){show(false);}

    void set_use_show_all_children(bool use=true){_use_show_all_children  = use;}
    bool get_use_show_all_children()const throw(){return _use_show_all_children;}

    bool is_visible()const throw(){return _visible;}

    /** \brief Returns false if either this widget or any of its parent containers is not visible.
     * */
    bool check_visibility()const;

    void set_tooltip_text(const Glib::ustring& str){_tooltip_text=str;}
    const Glib::ustring& get_tooltip_text()const throw(){return _tooltip_text;}

    void set_name(const Glib::ustring& name);
    const Glib::ustring& get_name()const throw(){return _name;}

    void manage(bool managed=true)throw(){g_assert(this);_is_managed=managed;}
    void unmanage(){manage(false);}
    bool get_is_managed()const throw(){g_assert(this);return _is_managed;}

    void set_memnotic(Glib::ustring::value_type m);
    Glib::ustring::value_type get_memnotic()const throw(){return _memnotic;}


    virtual void on_visibility_changed(){}

  private:
    Glib::ustring _tooltip_text, _name;
    Glib::ustring::value_type _memnotic;

    bool _is_managed, _visible, _use_show_all_children;

  //----Hierarchy----
  public:
    const Container* get_parent()const{return _parent;}
          Container* get_parent()     {return _parent;}

  private:
    friend class Container;
    Container* _parent;

  //----Engine-Stuff----
  public:
    typedef PaintTool EventExpose;
/*
    class EngineData : public Engine::EngineData
    {
    public:
      Widget* get_widget(){return _widget;}
      const Widget* get_widget()const{return _widget;}

      EngineData(Widget* widget) : _widget(widget)
      {
      }

      ~EngineData()throw()
      {
      }

    private:
      Widget* _widget;
    };*/

  public:
    /*virtual void on_realize();
    virtual void on_unrealize();*/

    virtual void set_theme(const Glib::RefPtr<Theme>& theme);
    const Glib::RefPtr<Theme>& get_theme()const throw(){return _theme;}

    virtual void on_expose(EventExpose& paint_tool);
    virtual void on_debug_draw(EventExpose& paint_tool)const;

    /*const Glib::RefPtr<const EngineData>  get_widget_engine_data()const throw(){return _engine_data;}
    const Glib::RefPtr<      EngineData>& get_widget_engine_data()      throw(){return _engine_data;}
    const Glib::RefPtr<const EngineData>  get_widget_engine_data_throw()const throw(NotRealized){if(!_engine_data)throw NotRealized();return _engine_data;}
    const Glib::RefPtr<      EngineData>& get_widget_engine_data_throw()      throw(NotRealized){if(!_engine_data)throw NotRealized();return _engine_data;}
*/
    void invalidate()
    {
      invalidate(_allocation);
    }
    void invalidate(Gdk::Rectangle rect)
    {
      rect.set_x(0);
      rect.set_y(0);
      Gdk::Region region(rect);
      invalidate(region);
    }
    void invalidate(Gdk::Region& region);

    virtual void on_invalidate(Gdk::Region& region){}

    void set_window_manager(WindowManager* wm);
    const ObsLink<WindowManager>& get_window_manager(){return _window_manager;}

  private:
    Glib::RefPtr<Theme> _theme;
    //Glib::RefPtr<EngineData> _engine_data;

    ObsLink<WindowManager> _window_manager;

  //----Location----
  public:
    typedef Gdk::Rectangle Allocation;

  public:
    /** \brief Overloadable getting called, when requesting size.
     *
     * Both, the width and the height must be at least 1 px long. Values smaller than 1 will be internally replaced with 1.
     *
     * \param w the minimal width, the widget requests.
     * \param h the minimal height, the widget requests.
     *
     * \note &w!=&h otherwise the behavior is undefined
     * */
    virtual void on_size_request(int& w, int& h){g_assert(&w!=&h);w=0;h=0;}
    virtual void on_size_request_changed(){}

    virtual void on_size_allocate(){}

  public:
    /** \brief Sets the minimum size, the Widget will have.
     *
     * This values can be smaller than the values got by on_size_request.
     *
     * Values <0 will be interpreted as unsetting the user definedsize request and using the value returned by on_size_request again.
     * */
    void set_size_request(int width, int height);
    void get_size_request(int& width, int& height)const throw(){g_assert(this);width=_size_request_width;height=_size_request_height;}
    int get_size_request_width()const throw(){g_assert(this);return _size_request_width;}
    int get_size_request_height()const throw(){g_assert(this);return _size_request_height;}

    int get_width()const throw(){return _allocation.get_width();}
    int get_height()const throw(){return _allocation.get_height();}

    void recalc_size_request();

    const Allocation& get_allocation()const throw(){g_assert(this);return _allocation;}

    bool is_custom_size_request()const throw(){return _custom_size_request_width&&_custom_size_request_height;}

  private:
    int _size_request_width, _size_request_height;
    bool _custom_size_request_width, _custom_size_request_height;

    Allocation _allocation;

  //----Events----
  public:
    class MouseEvent
    {
    public:

      /** \todo description
       * */
      int x;

      /** \todo description
       * */
      int y;
    };
    class MouseButtonEvent : public MouseEvent
    {
    public:
      /** \todo description
       * */
      int btn;

      /** \brief used to differ between single, double and triple clicks
       * */
      guint pressed_times;

      MouseButtonEvent(int btn, guint pressed_times);
    };

    typedef sigc::signal1<bool, MouseEvent&, InterruptableSigcAccumulator<false> > SignalMouseEvent;
    typedef sigc::signal1<bool, MouseButtonEvent&, InterruptableSigcAccumulator<false> > SignalMouseButtonEvent;

    SignalMouseEvent& signal_mouse_move(){return _signal_mouse_move;}
    SignalMouseEvent& signal_mouse_enter(){return _signal_mouse_enter;}
    SignalMouseEvent& signal_mouse_leave(){return _signal_mouse_leave;}
    SignalMouseButtonEvent& signal_button_press(){return _signal_button_press;}
    SignalMouseButtonEvent& signal_button_release(){return _signal_button_release;}

    virtual bool on_mouse_move(MouseEvent& mouse_event){return _signal_mouse_move.emit(mouse_event);}
    virtual bool on_mouse_enter(MouseEvent& mouse_event){return _signal_mouse_enter.emit(mouse_event);}
    virtual bool on_mouse_leave(MouseEvent& mouse_event){return _signal_mouse_leave.emit(mouse_event);}
    virtual bool on_button_press(MouseButtonEvent& mouse_event){return _signal_button_press.emit(mouse_event);}
    virtual bool on_button_release(MouseButtonEvent& mouse_event){return _signal_button_release.emit(mouse_event);}

    Widget* find_widget(MouseEvent& event);

    bool get_sensitive()const{return _sensitive;}
    void set_sensitive(bool s=true);

    bool can_process_mouse_move_events;
  private:
    bool can_process_mouse_drag_events;

  private:
    SignalMouseEvent _signal_mouse_move, _signal_mouse_enter, _signal_mouse_leave;
    SignalMouseButtonEvent _signal_button_press, _signal_button_release;

    bool _sensitive;

    /** \brief Removes any Focus.
     *
     * After calling this function, the widget will have no focus anymore: Neighter mouse nor keyfocus.
     * This Function gets called, when the widgets parent, visibility or sensibililty has changed.
     *
     * */
    void remove_any_focus();

  //----Ctors----
  public:
    Widget();
    ~Widget()throw();
  };
}

#include "./widgets/container.hpp"

#endif
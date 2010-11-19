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

#ifndef _SPACEDEMINER_UI_FRAMEWORK_CONTAINER_H_
#define _SPACEDEMINER_UI_FRAMEWORK_CONTAINER_H_

#include "./../widget.hpp"

namespace Framework
{
  /**
   *
   * \li When the destructor of your derived container is called, you have to make sure, EVERY children has ben removed
   * */
  class Container : public Widget
  {
  public:
    class WidgetHasAlreadyParent : public Glib::Exception
    {
    public:
      Glib::ustring what()const{return _("Trying to add widget, which already has a parent, to a container.");}

      ~WidgetHasAlreadyParent()throw(){}
    };
    class NoChild : public Glib::Exception
    {
    public:
      Glib::ustring what()const{return _("The container has no child");}

      ~NoChild()throw(){}
    };
    class NotAChild : public Glib::Exception
    {
    public:
      Glib::ustring what()const{return _("The widget is not a child");}

      ~NotAChild()throw(){}
    };
    class RemovingNotAChild : public Glib::Exception
    {
    public:
      Glib::ustring what()const{return _("A container trying to remove a widget, which isn't a child");}

      ~RemovingNotAChild()throw(){}
    };

    void show_all_children(bool recursive = true, bool show=true);
    void hide_all_children(bool recursive = true){return show_all_children(recursive, false);}

    void set_border_width(int border_width);
    int get_border_width()const throw(){return _border_width;}

    /** \brief wraps around v_rearrange_children
     * */
    void rearrange_children();

    void set_theme(const Glib::RefPtr<Theme>& theme);

    void on_size_request(int& w, int& h);

    void on_size_allocate();

    void on_children_invalidate(const Widget& child, Gdk::Region& region);
    void on_child_size_request_changed(const Widget& child);

    /**
     * \note the list won't be cleared before adding the container's children
     * */
    const std::list<Widget*>& get_children()const throw(){return _children;}

    /**
     *
     * \note You have to call the parents functino, too - otherwise the child will be inter still the child of the container
     * */
    virtual void remove(Widget& w);

    void remove_all()throw();

    void on_expose(EventExpose& paint_tool);
    void on_debug_draw(EventExpose& paint_tool)const;

    /**
     * \brief Gets called, when a visible child has been added/removed, or the visiblility of a child has changed.
     *
     * \note When deriving, call this function as last, for example:
     * \code
     *   class Foo : public Container
     *   {
     *     //...
     *   public:
     *     void on_child_visibility_changed(Widget& child)
     *     {
     *       std::cout<<"Hello "<<child.get_name().c_str()<<" :-)\n";
     *       Container::on_child_visibility_changed(child);
     *     }
     *   };
     * \endcode
     * */
    virtual void on_child_visibility_changed(Widget& child);

  protected:

    Widget* find_mouse_widget(const Widget::MouseEvent& mouse_event);

    enum ProxyFilter
    {
      PROXY_FILTER_NONE = 0,
      PROXY_FILTER_SWAP_XY = 0x0001<<0,
      PROXY_FILTER_SWAP_WH = 0x0001<<1,
    };

    friend ProxyFilter operator | (ProxyFilter a, ProxyFilter b){return ProxyFilter(int(a)|int(b));}
    friend ProxyFilter operator & (ProxyFilter a, ProxyFilter b){return ProxyFilter(int(a)&int(b));}

    void draw_childrens_debug(EventExpose& paint_tool)const;

    class ProxyContainer
    {
      ProxyContainer();
      ProxyContainer(const ProxyContainer&);

      Container& _container;
      bool _swap_wh;
    public:
      int get_width()const  throw(){return _swap_wh ? _container.get_allocation().get_height(): _container.get_allocation().get_width();}
      int get_height()const throw(){return _swap_wh ? _container.get_allocation().get_width() : _container.get_allocation().get_height();}

      int get_size_request_width()const throw(){return _swap_wh ? _container.get_size_request_height() : _container.get_size_request_width();}
      int get_size_request_height()const throw(){return _swap_wh ? _container.get_size_request_width() : _container.get_size_request_height();}

      int get_children_size_request_width(){return _swap_wh ? _container._children_size_request_height : _container._children_size_request_width;}
      int get_children_size_request_height(){return _swap_wh ? _container._children_size_request_width : _container._children_size_request_height;}

      Widget* get_self()throw(){return &_container;}
      const Widget* get_self()const throw(){return &_container;}

      ProxyContainer(Container& container, ProxyFilter filter) : _container(container), _swap_wh(filter&PROXY_FILTER_SWAP_WH)
      {
      }
    };

    class ProxySizeRequest
    {
      ProxySizeRequest();
      ProxySizeRequest(const ProxySizeRequest&);

      int *_w, *_h;

    public:
      int& w()throw(){g_assert(_w);return *_w;}
      int& h()throw(){g_assert(_h);return *_h;}
      const int& w()const throw(){g_assert(_w);return *_w;}
      const int& h()const throw(){g_assert(_h);return *_h;}

      ProxySizeRequest(int& w, int& h, ProxyFilter filter)
      {
        if(filter&PROXY_FILTER_SWAP_WH)
        {
          _w  = &h;
          _h  = &w;
        }else
        {
          _w  = &w;
          _h  = &h;
        }
      }
    };

    class ProxyChild
    {
      ProxyChild();
      ProxyChild(const ProxyChild&);

      Container& _container;
      Widget& _child;
      bool _swap_xy, _swap_wh;

    public:
      int get_x()const throw(){return _swap_xy ? _child.get_allocation().get_y() : _child.get_allocation().get_x();}
      int get_y()const throw(){return _swap_xy ? _child.get_allocation().get_x() : _child.get_allocation().get_y();}
      int get_width()const  throw(){return _swap_wh ? _child.get_allocation().get_height(): _child.get_allocation().get_width();}
      int get_height()const throw(){return _swap_wh ? _child.get_allocation().get_width() : _child.get_allocation().get_height();}

      int get_size_request_width()const throw(){return _swap_wh ? _child.get_size_request_height() : _child.get_size_request_width();}
      int get_size_request_height()const throw(){return _swap_wh ? _child.get_size_request_width() : _child.get_size_request_height();}

      Widget* get_self()throw(){return &_child;}
      const Widget* get_self()const throw(){return &_child;}

      void set_allocation(int x, int y, int width, int height)
      {
        if(_swap_xy)
          exchange(x, y);
        if(_swap_wh)
          exchange(width, height);

        if(_child.get_parent()!=&_container)
          throw std::logic_error("**Container::ProxyChild::set_allocation** The Parent of a Proxy Child is not allowed to be changed while lifetime");
        _container.set_child_allocation(_child, x, y, width, height);
      }

      Container& get_parent()throw(){return _container;}
      bool get_swap_xy()const throw(){return _swap_xy;}
      bool get_swap_wh()const throw(){return _swap_wh;}

      ProxyChild(Container& container, Widget& child, ProxyFilter filter) : _container(container), _child(child), _swap_xy(filter&PROXY_FILTER_SWAP_XY), _swap_wh(filter&PROXY_FILTER_SWAP_WH)
      {
        if(!&container)
          throw std::invalid_argument("**Container::ProxyChild::ProxyChild** container shouldn't be nullptr");
        if(!&child)
          throw std::invalid_argument("**Container::ProxyChild::ProxyChild** child shouldn't be nullptr");
        if(_child.get_parent()!=&_container)
          throw std::logic_error("**Container::ProxyChild::ProxyChild** The Parent of child should be container");
      }
      ~ProxyChild()
      {
        if(_child.get_parent()!=&_container)
          throw std::logic_error("**Container::ProxyChild::~ProxyChild** The Parent of a Proxy Child is not allowed to be changed while lifetime");
      }
    };

  protected:
    /**
     * _children_size_request_width can be used by you to store the childrens size if needed - will be set by constructor to 0 and never be touched again - use it for your own purposes
     * */
    int _children_size_request_width;
    /**
     * \see _children_size_request_width
     * */
    int _children_size_request_height;

    void validate_child_(Widget&);
    void add_child_(Widget&);
    void set_child_allocation(Widget& child, int x, int y, int w, int h)
    {
      set_child_allocation(child, Gdk::Rectangle(x, y, w, h));
    }
    void set_child_allocation(Widget& child, const Gdk::Rectangle& new_alloc);

    /**
     * \note NEVER call this by yourself, use the rearrange_children wrapper instead!
     * */
    virtual void v_rearrange_children();

    Container();
    ~Container()throw();

  private:
    int _border_width;

    std::list<Widget*> _children;
  };

  inline bool Widget::check_visibility()const
  {
    if(!is_visible())
      return false;
    if(!get_parent())
      return true;
    return get_parent()->check_visibility();
  }
}

#endif
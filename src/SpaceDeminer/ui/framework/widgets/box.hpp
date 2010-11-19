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

#ifndef _SPACEDEMINER_UI_FRAMEWORK_BOX_H_
#define _SPACEDEMINER_UI_FRAMEWORK_BOX_H_

#include "./container.hpp"

namespace Framework
{
  class Box : public Container
  {
  public:
    void set_spacing(int s)
    {
      _spacing=s;
      recalc_size_request();
    }
    int get_spacing()const throw(){return _spacing;}

    void pack_start(Widget& child, bool expand=true, bool fill=true, int padding=0);
    void pack_end(Widget& child, bool expand=true, bool fill=true, int padding=0);

    void remove(Widget& w);

    bool get_is_horizontal()const throw(){return _horizontal;}

    ~Box()throw();

  protected:
    Box(bool h, bool is_button_box=false);

    void on_size_request(int& w, int& h);

    void on_child_visibility_changed(Widget& child);

    void v_rearrange_children();

    int _expandable_childrens_size_request;

  protected:
    class ProxySizeRequest : public Container::ProxySizeRequest
    {
    public:
      ProxySizeRequest(Box& box, int& w, int& h) : Container::ProxySizeRequest(w, h, box.get_is_horizontal() ? PROXY_FILTER_SWAP_WH : PROXY_FILTER_NONE)
      {
      }
    };

    class ProxyContainer : public Container::ProxyContainer
    {
    public:
      ProxyContainer(Box& box) : Container::ProxyContainer(box, box.get_is_horizontal() ? PROXY_FILTER_SWAP_WH : PROXY_FILTER_NONE)
      {
      }
    };

    class ProxyChild : public Container::ProxyChild
    {
    public:
      ProxyChild(Box& box, Widget& child) : Container::ProxyChild(box, child, box.get_is_horizontal() ? PROXY_FILTER_SWAP_XY|PROXY_FILTER_SWAP_WH : PROXY_FILTER_NONE)
      {
      }
    };

  protected:
    struct BoxChildContainer
    {
      int padding;
      bool expand;
      bool fill;
      Widget* child;

      BoxChildContainer(Widget* c)
      {
        child=c;
      }
    };

    typedef std::list<BoxChildContainer> BoxChildContainerList;

    BoxChildContainerList _start_children, _end_children;

    int get_n_visible_expandable_children()const throw(){return _n_visible_expandable_children;}

#ifdef NDEBUG
    enum InvariantTests_
    {
      TEST_GENERAL = 0,
      TEST_EXPANDABLE_VISIBLE_COUNT = 0x0001 | TEST_GENERAL,
    };
    void testInvariant(InvariantTests_ tests=TEST_GENERAL)const throw();
#endif

  protected:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  //don't use
    int _spacing, _n_visible_expandable_children;
    const bool _horizontal;
    const bool _is_button_box;

    void clear(BoxChildContainerList& l)throw();
#endif
  };

  class VBox : public Box
  {
  public:
    VBox();
    ~VBox()throw();
  };

  class HBox : public Box
  {
  public:
    HBox();
    ~HBox()throw();
  };
}

#endif
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

#include "./buttonbox.hpp"

namespace Framework
{
  ButtonBox::ButtonBox(bool h) : Box(h, true)
  {
    set_name("buttonbox");

    _min_max_child_width  = 0;
    _min_max_child_height  = 0;
  }

  ButtonBox::~ButtonBox()throw()
  {
  }

  void ButtonBox::set_layout(ButtonBoxStyle layout)
  {
    _layout = layout;
    invalidate();
  }

  void ButtonBox::set_theme(const Glib::RefPtr<Theme>& theme)
  {
    ParentClass::set_theme(theme);

    if(theme)
    {
      Theme::Metrics m;

      theme->get_metrics("min-size/ButtonBox/Button/"+get_name(), m);
      _min_max_child_width = abs(m.x1-m.x2);
      _min_max_child_height = abs(m.y1-m.y2);

    }else
    {
      _min_max_child_width = 0;
      _min_max_child_height = 0;
    }

    recalc_size_request();
  }

  void ButtonBox::on_size_request(int& w_, int& h_)
  {
#ifdef NDEBUG
    testInvariant(TEST_EXPANDABLE_VISIBLE_COUNT);
#endif

    Container::on_size_request(w_, h_);

    ProxySizeRequest proxy_size_request(*this, w_, h_);

    _expandable_childrens_size_request  = 0;

    if(get_children().size()==0)
      return;

    _n_visible_children=0;

    _max_child_width  = _min_max_child_width;
    _max_child_height = _min_max_child_height;

    if(get_is_horizontal())
      exchange(_max_child_width, _max_child_height);

    for(BoxChildContainerList::iterator iter = _start_children.begin(); iter!=_end_children.end();)
    {
      if(iter==_start_children.end())
      {
        iter  = _end_children.begin();
        continue;
      }

      if(iter->child->is_visible())
      {
        ++_n_visible_children;

        ProxyChild proxy_child(*this, *iter->child);

        _max_child_width = MAX(_max_child_width , proxy_child.get_size_request_width());
        _max_child_height = MAX(_max_child_height , proxy_child.get_size_request_height());
      }

      ++iter;
    }

    if(get_layout()==BUTTONBOX_SPREAD)
      proxy_size_request.h() += (_n_visible_children+1) * _spacing;
    else
      proxy_size_request.h() += (_n_visible_children-1) * _spacing;
    proxy_size_request.h() += _max_child_height*_n_visible_children;
    proxy_size_request.w() += _max_child_width;

    _children_size_request_width  = w_;
    _children_size_request_height  = h_;

    if(get_is_horizontal())
    {
      exchange(_max_child_width, _max_child_height);
    }

#ifdef NDEBUG
    testInvariant(TEST_EXPANDABLE_VISIBLE_COUNT);
#endif
  }

  void ButtonBox::v_rearrange_children()
  {
#ifdef NDEBUG
    testInvariant(TEST_EXPANDABLE_VISIBLE_COUNT);
#endif

    ProxyContainer proxy_container(*this);

    if(is_custom_size_request())
    {
      int a, b;
      on_size_request(a,b);
    }

    if(_n_visible_children==0)
      return;

    Real inv_childrens_size_request = 1.f/Real(_expandable_childrens_size_request);
    Real additional_size = MAX(0.f, Real(proxy_container.get_height() - proxy_container.get_children_size_request_height()));
    Real single_space = additional_size;
    int x_pos;

    int max_child_w;
    int max_child_h;

    if(get_is_horizontal())
    {
      max_child_w = MAX(_max_child_height, _max_child_height);
      max_child_h = MAX(_max_child_width, _max_child_width);
    }else
    {
      max_child_w = MAX(_max_child_width, _max_child_width);
      max_child_h = MAX(_max_child_height, _max_child_height);
    }

    Real cursor  = get_border_width();

    ButtonBoxStyle l = get_layout();
    switch(get_layout())
    {
    case BUTTONBOX_EDGE:
      single_space = additional_size / Real(_n_visible_children-1);
      break;
    case BUTTONBOX_START:
      break;
    case BUTTONBOX_END:
      cursor  += additional_size;
      break;
    case BUTTONBOX_CENTER:
      cursor  += additional_size*0.5f;
      break;
    case BUTTONBOX_SPREAD:
    default:
      single_space = additional_size / Real(_n_visible_children+1);
    }

    x_pos = MAX(get_border_width(), (proxy_container.get_width()-max_child_w)/2);

    int i=0;

    for(BoxChildContainerList::iterator iter = _start_children.begin(); iter!=_end_children.end();)
    {
      if(iter==_start_children.end())
      {
        iter  = _end_children.begin();
        continue;
      }

      g_assert(iter->child);
      g_assert(iter->child->get_size_request_width()>=0);
      g_assert(iter->child->get_size_request_height()>=0);

      ProxyChild child(*this, *iter->child);

      if(iter->child->is_visible())
      {
        switch(get_layout())
        {
        case BUTTONBOX_EDGE:
          break;
        case BUTTONBOX_START:
        case BUTTONBOX_END:
        case BUTTONBOX_CENTER:
          break;
        case BUTTONBOX_SPREAD:
        default:
          cursor  += single_space;
          cursor  += get_spacing();
        }

        child.set_allocation(x_pos, round(cursor), max_child_w, max_child_h);
        cursor  += max_child_h;

        switch(get_layout())
        {
        case BUTTONBOX_EDGE:
          cursor  += single_space;
          cursor  += get_spacing();
          break;
        case BUTTONBOX_START:
        case BUTTONBOX_END:
        case BUTTONBOX_CENTER:
          cursor  += get_spacing();
          break;
        case BUTTONBOX_SPREAD:
        default:
          break;
        }

        ++i;
      }
      ++iter;
    }
  }

  //----

  HButtonBox::HButtonBox() : ButtonBox(true)
  {
    set_name("hbuttonbox");
  }

  HButtonBox::~HButtonBox()throw()
  {
  }

  //----

  VButtonBox::VButtonBox() : ButtonBox(false)
  {
    set_name("vbuttonbox");
  }

  VButtonBox::~VButtonBox()throw()
  {
  }
}

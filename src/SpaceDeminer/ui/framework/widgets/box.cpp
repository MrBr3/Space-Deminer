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

#include "./box.hpp"

namespace Framework
{
  Box::Box(bool h, bool is_button_box) : _horizontal(h), _is_button_box(is_button_box)
  {
    _spacing  = 0;
    _expandable_childrens_size_request = 0;
    _n_visible_expandable_children  = 0;
    set_name("box");
  }

  Box::~Box()throw()
  {
  }

#ifdef NDEBUG
  void Box::testInvariant(InvariantTests_ test)const throw()
  {
    try
    {
      if((test&TEST_GENERAL) == TEST_GENERAL)
      {
        if(get_children().size() != _start_children.size() + _end_children.size())
        {
          std::cout<<"get_children().size() "<<get_children().size()<<"; _start_children.size() "<<_start_children.size()<<"; _end_children.size() "<<_end_children.size()<<";\n";
        }
        g_assert(get_children().size() == _start_children.size() + _end_children.size());
      }else
        g_assert_not_reached();

      if((test&TEST_EXPANDABLE_VISIBLE_COUNT) == TEST_EXPANDABLE_VISIBLE_COUNT)
      {
        int _n_visible  = 0;
        int _n_expandable = 0;
        int _n_visible_expandable = 0;
        for(BoxChildContainerList::const_iterator iter = _start_children.begin(); iter!=_end_children.end();)
        {
          if(iter==_start_children.end())
          {
            iter  = _end_children.begin();
            continue;
          }

          g_assert(iter->child);
          if(iter->child->is_visible())
            ++_n_visible;
          if(iter->expand)
            ++_n_expandable;
          if(iter->child->is_visible() && iter->expand)
            ++_n_visible_expandable;

           ++iter;
        }

        g_assert(_n_visible_expandable<=_n_visible);
        g_assert(_n_visible_expandable<=_n_expandable);
        g_assert(_n_visible_expandable==get_n_visible_expandable_children());
      }
    }catch(...)
    {
      g_assert_not_reached();
    }
  }
#endif

  void Box::pack_start(Widget& child, bool expand, bool fill, int padding)
  {
    validate_child_(child);

    _start_children.push_back(&child);
    BoxChildContainer& cc = *_start_children.rbegin();

    g_assert(cc.child==&child);
    cc.fill  = fill;
    cc.expand  = expand;
    cc.padding  = _is_button_box ? 0 : padding;

    Container::add_child_(child);

#ifdef NDEBUG
    testInvariant(TEST_EXPANDABLE_VISIBLE_COUNT);
#endif
  }

  void Box::pack_end(Widget& child, bool expand, bool fill, int padding)
  {
    validate_child_(child);

    _end_children.push_back(&child);
    BoxChildContainer& cc = *_end_children.rbegin();

    g_assert(cc.child==&child);
    cc.fill  = fill;
    cc.expand  = expand;
    cc.padding  = _is_button_box ? 0 : padding;

    Container::add_child_(child);

#ifdef NDEBUG
    testInvariant(TEST_EXPANDABLE_VISIBLE_COUNT);
#endif
  }

  void Box::on_child_visibility_changed(Widget& child)
  {
    int _n_visible  = 0;
    int _n_visible_expandable = 0;
    for(BoxChildContainerList::const_iterator iter = _start_children.begin(); iter!=_end_children.end();)
    {
      if(iter==_start_children.end())
      {
        iter  = _end_children.begin();
        continue;
      }

      g_assert(iter->child);
      if(iter->child->is_visible())
        ++_n_visible;
      if(iter->child->is_visible() && iter->expand)
        ++_n_visible_expandable;

      ++iter;
    }

    _n_visible_expandable_children  = _n_visible_expandable;

    Container::on_child_visibility_changed(child);

#ifdef NDEBUG
    testInvariant(TEST_EXPANDABLE_VISIBLE_COUNT);
#endif
  }

  void Box::on_size_request(int& w_, int& h_)
  {
#ifdef NDEBUG
    testInvariant(TEST_EXPANDABLE_VISIBLE_COUNT);
#endif

    Container::on_size_request(w_, h_);

    ProxySizeRequest proxy_size_request(*this, w_, h_);

    int max_ortho=0;

    _expandable_childrens_size_request  = 0;

    if(get_children().size()==0)
      return;

    bool is_first_visible = true;

    for(BoxChildContainerList::iterator iter = _start_children.begin(); iter!=_end_children.end();)
    {
      if(iter==_start_children.end())
      {
        iter  = _end_children.begin();
        continue;
      }

      if(iter->child->is_visible())
      {
        if(!is_first_visible)
          proxy_size_request.h() += _spacing;

        ProxyChild proxy_child(*this, *iter->child);

        proxy_size_request.h() += iter->padding*2 + proxy_child.get_size_request_height();
        if(iter->expand)
        {
          _expandable_childrens_size_request += proxy_child.get_size_request_height();
        }

        max_ortho = MAX(max_ortho, proxy_child.get_size_request_width());

        is_first_visible  = false;
      }

      ++iter;
    }

    proxy_size_request.w() += max_ortho;

    _children_size_request_width  = w_;
    _children_size_request_height  = h_;

#ifdef NDEBUG
    testInvariant(TEST_EXPANDABLE_VISIBLE_COUNT);
#endif
  }

  void Box::remove(Widget& w)
  {
    bool found  = false;

    for(BoxChildContainerList::iterator iter  = _start_children.begin(); iter!=_start_children.end(); ++iter)
    {
      g_assert(iter->child);
      g_assert(iter->child->get_parent()==this);
      if(iter->child == &w)
      {
        _start_children.erase(iter);
        found = true;
        break;
      }
    }
    for(BoxChildContainerList::iterator iter  = _end_children.begin(); !found && iter!=_end_children.end(); ++iter)
    {
      g_assert(iter->child);
      g_assert(iter->child->get_parent()==this);
      if(iter->child == &w)
      {
        _end_children.erase(iter);
        found = true;
        break;
      }
    }

    if(!found)
    {
      g_assert(w.get_parent()!=this);
      throw NotAChild();
    }

    Container::remove(w);

#ifdef NDEBUG
    testInvariant(TEST_EXPANDABLE_VISIBLE_COUNT);
#endif
  }

  void Box::v_rearrange_children()
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

    Real inv_childrens_size_request = 1.f/Real(_expandable_childrens_size_request);
    Real additional_size = MAX(0.f, Real(proxy_container.get_height() - proxy_container.get_children_size_request_height()));
    Real total_carryover = 0.f;

    int cursor  = get_border_width();

    for(BoxChildContainerList::iterator iter = _start_children.begin(); iter!=_start_children.end(); ++iter)
    {
      if(!iter->child->is_visible())
        continue;

      int child_x, child_y, child_w, child_h;

      g_assert(iter->child);
      g_assert(iter->child->get_size_request_width()>=0);
      g_assert(iter->child->get_size_request_height()>=0);

      ProxyChild child(*this, *iter->child);

      Real expand_size = 0;
      int iexpand_size = 0;

      if(iter->expand)
      {
        g_assert(get_n_visible_expandable_children());
        if(_expandable_childrens_size_request)
          expand_size = additional_size*Real(child.get_size_request_height())*inv_childrens_size_request;
        else
          expand_size = additional_size/Real(get_n_visible_expandable_children());

        g_assert(expand_size>=0);

        if(get_frac_part(expand_size))
        {
          Real epsilon  = get_frac_part(expand_size)*0.001f;
          total_carryover-=get_frac_part(expand_size);

          if(total_carryover<-epsilon)
          {
            total_carryover += 1.f;
            expand_size+=1.f;
          }
        }
      }

      cursor  += iter->padding;

      child_x = get_border_width();
      child_w = MAX(child.get_size_request_width(), proxy_container.get_width()-get_border_width()*2);

      g_assert(child_w>=child.get_size_request_width());

      iexpand_size  = expand_size;

      g_assert(child.get_size_request_height()>=0);
      g_assert(iexpand_size>=0);

      if(iter->fill)
      {
        child_y = cursor;
        child_h = iexpand_size+child.get_size_request_height();
      }else
      {
        child_y = cursor + (iexpand_size>>1);
        child_h = child.get_size_request_height();
      }

      g_assert(child_h>=0);
      g_assert(child_h>=child.get_size_request_height());

      child.set_allocation(child_x, child_y, child_w, child_h);

      cursor  += iexpand_size+child.get_size_request_height()+iter->padding+get_spacing();
    }
    cursor  = proxy_container.get_height()-get_border_width();
    for(BoxChildContainerList::iterator iter = _end_children.begin(); iter!=_end_children.end(); ++iter)
    {
      if(!iter->child->is_visible())
        continue;

      int child_x, child_y, child_w, child_h;

      g_assert(iter->child);
      g_assert(iter->child->get_size_request_width()>=0);
      g_assert(iter->child->get_size_request_height()>=0);

      ProxyChild child(*this, *iter->child);

      Real expand_size = 0.f;
      int iexpand_size = 0;

      if(iter->expand)
      {
        g_assert(get_n_visible_expandable_children());
        if(_expandable_childrens_size_request)
          expand_size = additional_size*Real(child.get_size_request_height())*inv_childrens_size_request;
        else
          expand_size = additional_size/Real(get_n_visible_expandable_children());

        if(get_frac_part(expand_size))
        {
          Real epsilon  = get_frac_part(expand_size)*0.001f;
          total_carryover-=get_frac_part(expand_size);

          if(total_carryover<-epsilon)
          {
            total_carryover += 1.f;
            expand_size+=1.f;
          }
        }
      }

      cursor  -= iter->padding;

      child_x = get_border_width();
      child_w = MAX(child.get_size_request_width(), proxy_container.get_width()-get_border_width()*2);

      iexpand_size  = expand_size;

      g_assert(child.get_size_request_height()>=0);
      g_assert(iexpand_size>=0);

      if(iter->fill)
      {
        child_h = iexpand_size+child.get_size_request_height();
        child_y = cursor-child_h;
      }else
      {
        child_h = child.get_size_request_height();
        child_y = cursor - (iexpand_size>>1) - child_h;
      }

      g_assert(child_h>=0);

      child.set_allocation(child_x, child_y, child_w, child_h);

      cursor  -= iexpand_size+child.get_size_request_height()+iter->padding+get_spacing();
    }
  }

  //--------

  VBox::VBox() : Box(false)
  {
    set_name("vbox");
  }

  VBox::~VBox()throw()
  {
  }

  //--------

  HBox::HBox() : Box(true)
  {
    set_name("hbox");
  }

  HBox::~HBox()throw()
  {
  }

}
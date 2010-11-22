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

#include "./bin.hpp"
#include "./label.hpp"
#include "./imagewidget.hpp"

namespace Framework
{
  Bin::Bin()
  {
    _child  = nullptr;
    _private_child = nullptr;
    set_name("box");
  }

  Bin::~Bin()throw()
  {
    if(_private_child)
    {
      g_assert(_private_child==_child);
      remove();
    }
    g_assert(!_private_child);
  }

  void Bin::on_size_request(int& w, int& h)
  {
    Container::on_size_request(w, h);

    if(_child)
    {
      w += _child->get_size_request_width();
      h += _child->get_size_request_height();

      _children_size_request_width  = _child->get_size_request_width();
      _children_size_request_height  = _child->get_size_request_height();
    }else
    {
      _children_size_request_width=_children_size_request_height=0;
    }
  }

  void Bin::remove(Widget& w)
  {
    if(&w!=get_child())
      throw NotAChild();

    remove();
  }

  void Bin::remove()
  {
    if(!_child)
      throw NoChild();

    g_assert(_private_child==_child || !_private_child);

    Container::remove(*_child);

    _child  = nullptr;

    if(_private_child)
    {
      delete _private_child;
      _private_child  = nullptr;
    }
  }

  void Bin::add(Widget& widget)
  {
    validate_child_(widget);

    if(get_child())
      throw BinHasAlreadyChild();

    g_assert(!_private_child);

    _child  = &widget;

    Container::add_child_(widget);
  }

  void Bin::set_label(const Glib::ustring& label, bool use_memnotic, Real xalign, Real yalign)
  {
    if(get_child())
      throw BinHasAlreadyChild();

    Framework::Label* labelwidget  = new Framework::Label(label, use_memnotic);
    labelwidget->set_alignment(xalign, yalign);
    labelwidget->show();

    add(*labelwidget);

    _private_child  = labelwidget;
  }

  void Bin::set_image(const ResPtr<Image>& image, Real xalign, Real yalign)
  {
    if(get_child())
      throw BinHasAlreadyChild();

    Framework::ImageWidget* imagewidget  = new Framework::ImageWidget(image);
    imagewidget->set_alignment(xalign, yalign);
    imagewidget->show();
    imagewidget->set(image);

    add(*imagewidget);

    _private_child  = imagewidget;
  }
}
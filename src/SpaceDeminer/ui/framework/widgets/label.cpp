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

#include "./label.hpp"

namespace Framework
{
  Label::Label()
  {
    set_name("label");
  }

  Label::Label(const Glib::ustring& label, bool use_memnotic) : _label(label), _memnotic(use_memnotic)
  {
    set_name("label");
  }

  Label::~Label()throw()
  {
  }

  void Label::set_theme(const Glib::RefPtr<Theme>& theme)
  {
    Misc::set_theme(theme);

    if(theme)
      _font = theme->create_font("Label/Label/"+get_name());

    if(!theme)
      _font.reset();

    recalc_size_request();
  }

  void Label::on_size_request(int& w, int& h)
  {
    if(!_font)
    {
      w = 0;
      h = 0;
      return;
    }

    _font->get_size(_label, w, h);
  }

  void Label::on_expose(EventExpose& paint_tool)
  {
    Misc::on_expose(paint_tool);

    if(!_font)
      return;

    int w=get_size_request_width(), h=get_size_request_height();

    _font->draw_simple(paint_tool, _label, round(get_width()*get_xalign()), round(get_height()*get_yalign()), get_xalign(), get_yalign(), &w, &h);
  }

  void Label::set_label(const Glib::ustring& label, bool use_memnotic)
  {
    _memnotic = use_memnotic;
    _label  = label;

    recalc_size_request();
    invalidate();
  }
}
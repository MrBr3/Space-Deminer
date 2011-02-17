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

#include "./dialog.hpp"
#include <gtkmm.h>

namespace Framework
{
  Dialog::Dialog()
  {
    _response = NO_RESPONSE;
    _fade_other_out = true;

    set_name("dialog");

    add(alignment);

    alignment.add(windowframe);
    alignment.set(0.5f, 0.5f, 0.0f, 0.0f);

    windowframe.add(dlg_vbox);

    dlg_vbox.pack_end(actionarea, false, false);
    actionarea.set_layout(BUTTONBOX_END);

    dlg_vbox.pack_end(separator, false, false);

    show_all_children();
  }

  Dialog::~Dialog()throw()
  {
  }

  void Dialog::set_theme(const Glib::RefPtr<Theme>& theme)
  {
    ParentClass::set_theme(theme);

    if(theme)
    {
      dlg_vbox.remove(actionarea);
      dlg_vbox.remove(separator);
      dlg_vbox.pack_end(actionarea, false, false, theme->get_spacing(Theme::SPACING_NORMAL));
      dlg_vbox.pack_end(separator, false, false);
      dlg_vbox.set_border_width(theme->get_spacing(Theme::SPACING_LARGE));
      actionarea.set_spacing(theme->get_spacing(Theme::SPACING_NORMAL));
    }
  }

  void Dialog::set_fade_other_out(bool b)
  {
    _fade_other_out = b;
    invalidate();
  }

  Button* Dialog::add_button(const Glib::ustring& text, int response_id, bool use_memnotics)
  {
    Button* btn = new Button(text, use_memnotics);

    btn->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &Dialog::response), response_id));
    btn->show();
    btn->manage();

    actionarea.pack_start(*btn);

    return btn;
  }

  void Dialog::response(int r)
  {
    _response = r;

    hide();

    signal_response().emit(r);

    std::map<int, sigc::signal<void> >::iterator iter = _signal_certain_response.find(r);
    if(iter!=_signal_certain_response.end())
      iter->second.emit();
  }

  void Dialog::run()
  {
    if(!is_registered())
      throw std::logic_error("**Dialog::run** Can't run a not registered Dialog");

    _response = NO_RESPONSE;

    show();
  }

  void Dialog::on_expose(EventExpose& paint_tool)
  {
    if(_fade_other_out)
      paint_tool.draw_color_rect(0, 0, get_width(), get_height(), 0.f, 0.f, 0.f, 0.5f);

    ParentClass::on_expose(paint_tool);
  }
}

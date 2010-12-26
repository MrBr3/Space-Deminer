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

#include "./MainWindow.hpp"

PanelLabelWidget::DoubleSep::DoubleSep()
{
  add(vbox);
  vbox.pack_start(s1, false, false, 1);
  vbox.pack_start(s2, false, false, 1);

  set(0.5, 0.6, 1., 0.);

  set_size_request(16, -1);

  vbox.show();
  s1.show();
  s2.show();
}

PanelLabelWidget::PanelLabelWidget()
{
  pack_start(s1);
  pack_start(_caption, false, false);
  pack_start(s2);
  s1.show();
  _caption.show();
  s2.show();
  set_spacing(LENGTH_SMALLSPACE);
}

PanelLabelWidget::~PanelLabelWidget()throw()
{
}

void PanelLabelWidget::set_main_caption(const Glib::ustring& str)
{
  _main_caption = str;
  _update_widget();
}

void PanelLabelWidget::set_sub_caption(const Glib::ustring& str)
{
  _sub_caption = str;
  _update_widget();
}

void PanelLabelWidget::_update_widget()
{
  if(_sub_caption.empty())
    _caption.set_markup(Glib::ustring::compose("<b>%1</b>", _main_caption));
  else
    _caption.set_markup(Glib::ustring::compose("<b>%1</b> [%2]", _main_caption, _sub_caption));
}

//========================================
//========================================

SettingsWidget::SettingsWidget()
{
  pack_start(caption, false, false);
  caption.show();

  set_spacing(LENGTH_SMALLSPACE);
}

SettingsWidget::~SettingsWidget()throw()
{
}
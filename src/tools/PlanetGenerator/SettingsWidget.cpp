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

SettingsWidget* SettingsWidget::visible_settings  = nullptr;

SettingsWidget::SettingsWidget()
{
  pack_start(caption, false, false);
  caption.show();

  pack_start(scroll, true, true);
  scroll.show();
  scroll.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);

  scroll.add(table);
  table.show();
  table.set_border_width(LENGTH_SMALLSPACE);
  table.set_spacings(LENGTH_SMALLSPACE);
  n_entries = 0;

  set_spacing(LENGTH_SMALLSPACE);

  Gtk::manage(this);

  if(main_window)
    main_window->append_settings_widget(*this);
}

SettingsWidget::~SettingsWidget()throw()
{
  if(visible_settings==this)
    visible_settings  = nullptr;
}

void SettingsWidget::bring_to_front()
{
  g_assert(this);

  if(visible_settings==this)
    return;
  if(visible_settings)
    visible_settings->hide();

  show();

  visible_settings  = this;
}

void SettingsWidget::append_color_widget(const Glib::ustring& name, const Glib::ustring& label, const Glib::ustring& tooltip, const sigc::slot<Gdk::Color>& getter, const sigc::slot<void, const Gdk::Color&>& setter, sigc::signal<void>& signal_changed)
{
  Gtk::Label* wlabel  = Gtk::manage(new Gtk::Label(label));
  Gtk::ColorButton* color_button  = Gtk::manage(new Gtk::ColorButton);

  wlabel->set_alignment(0., 0.5);
  wlabel->show();

  sigc::slot<Gdk::Color> w_getter = sigc::mem_fun(*color_button, &Gtk::ColorButton::get_color);
  sigc::slot<void, const Gdk::Color&> w_setter = sigc::mem_fun(*color_button, &Gtk::ColorButton::set_color);

  w_setter(getter());

  color_button->show();
  color_button->set_tooltip_text(tooltip);
  color_button->set_use_alpha(false);
  color_button->signal_color_set().connect(create_updater(w_getter, setter));
  signal_changed.connect(create_updater(getter, w_setter));

  table.attach(*wlabel, 0, 1, n_entries, n_entries+1, Gtk::FILL, Gtk::FILL);
  table.attach(*color_button, 1, 2, n_entries, n_entries+1, Gtk::EXPAND|Gtk::FILL, Gtk::FILL);
  ++n_entries;
}

void SettingsWidget::append_boolean_widget(const Glib::ustring& name, const Glib::ustring& label, const Glib::ustring& tooltip, const sigc::slot<bool>& getter, const sigc::slot<void, bool>& setter, sigc::signal<void>& signal_changed)
{
  Gtk::CheckButton* check_button  = Gtk::manage(new Gtk::CheckButton);

  sigc::slot<bool> w_getter = sigc::mem_fun(*check_button, &Gtk::CheckButton::get_active);
  sigc::slot<void, bool> w_setter = sigc::mem_fun(*check_button, &Gtk::CheckButton::set_active);

  w_setter(getter());

  check_button->show();
  check_button->set_label(label);
  check_button->set_tooltip_text(tooltip);
  check_button->signal_toggled().connect(create_updater(w_getter, setter));
  signal_changed.connect(create_updater(getter, w_setter));

  table.attach(*check_button, 0, 2, n_entries, n_entries+1, Gtk::EXPAND|Gtk::FILL, Gtk::FILL);
  ++n_entries;
}
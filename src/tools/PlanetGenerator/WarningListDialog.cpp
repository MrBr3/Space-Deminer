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

WarningListDialog::WarningListDialog()
{
  set_title(_("Warning"));

  g_assert(get_vbox());
  get_vbox()->pack_start(scrolled);

  scrolled.set_border_width(LENGTH_BORDER_WIDTH);

  scrolled.add(warnings);
  scrolled.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_ALWAYS);

  try
  {
    warning_icon  = Gdk::Pixbuf::create_from_file(apply_filename_macros("$(exe-share)/icons/scalable/dialog-warning.svg"), 22, 22);
  }CATCH_ALL("**WarningListDialog::WarningListDialog**", NOTHING_MACRO);

  set_default_size(320, 480);

  show_all_children();
}

void WarningListDialog::add_warning(const Glib::ustring& warning)
{
  Gtk::HBox* hb = Gtk::manage(new Gtk::HBox);
  Gtk::Label* l = Gtk::manage(new Gtk::Label);
  hb->show();
  hb->set_spacing(LENGTH_BORDER_WIDTH*2);

  if(warning_icon)
  {
    Gtk::Image* img = Gtk::manage(new Gtk::Image(warning_icon));
    hb->pack_start(*img, false, false);
  }
  l->set_markup(warning);
  l->set_alignment(0., 0.5);
  l->set_line_wrap(true);
  hb->pack_start(*l);

  if(warnings.get_children().size())
  {
    Gtk::HSeparator* s = Gtk::manage(new Gtk::HSeparator);
    s->show();
    warnings.pack_start(*s, false, false, LENGTH_SMALLSPACE);
  }

  hb->show_all_children();
  warnings.pack_start(*hb, false, false);
}

int WarningListDialog::go(const std::list<Glib::ustring>& warnings, const Glib::ustring& user_button)
{
  if(warnings.size()==0)
    return OK;

  WarningListDialog dlg;

  dlg.add_button(Gtk::Stock::OK, OK);
  if(!user_button.empty())
    dlg.add_button(user_button, USER);
  dlg.add_button(Gtk::Stock::CANCEL, CANCEL);

  for(std::list<Glib::ustring>::const_iterator i = warnings.begin(); i!=warnings.end(); ++i)
    dlg.add_warning(*i);

  return dlg.run();
}
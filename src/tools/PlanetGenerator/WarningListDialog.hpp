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

class WarningListDialog : public Gtk::Dialog
{
  WarningListDialog();

  Gtk::ScrolledWindow scrolled;
  Gtk::VBox warnings;
  Glib::RefPtr<Gdk::Pixbuf> warning_icon;

  void add_warning(const Glib::ustring& warning);

public:
  static const int OK = 0;
  static const int CANCEL = 1;
  static const int USER = 2;

  static int go(const std::list<Glib::ustring>& warnings, const Glib::ustring& user_button=Glib::ustring());
};
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

class ImageFileSettings : public Gtk::Expander
{
  typedef Gtk::Expander ParentClass;

private:
  Glib::RefPtr<ImageFile> image_file;

  Gtk::Label _label;
  Gtk::HBox _hbox;
  Gtk::Table table;

  guint _n_entries;

public:
  ImageFileSettings(const Glib::RefPtr<ImageFile>& imagefile, const Glib::ustring& name, const Glib::ustring& label, const Glib::ustring& tooltip);
  ~ImageFileSettings()throw();
};

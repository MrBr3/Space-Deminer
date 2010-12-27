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

class PanelLabelWidget : public Gtk::HBox
{
public:
  typedef Gtk::HBox ParentClass;

  class DoubleSep : public Gtk::Alignment
  {
    Gtk::VBox vbox;
    Gtk::HSeparator s1, s2;

  public:
  DoubleSep();
  };

private:
  Gtk::Label _caption;
  DoubleSep s1, s2;
  Glib::ustring _main_caption, _sub_caption;

  void _update_widget();

public:
  void set_main_caption(const Glib::ustring& str);
  void set_sub_caption(const Glib::ustring& str);

  PanelLabelWidget();
  ~PanelLabelWidget()throw();
};

class SettingsWidget : public Gtk::VBox
{
  typedef Gtk::VBox ParentClass;

public:

  PanelLabelWidget caption;

  void set_main_caption(const Glib::ustring& str){caption.set_main_caption(str);}
  void set_sub_caption(const Glib::ustring& str){caption.set_sub_caption(str);}

public:
  SettingsWidget();
  ~SettingsWidget()throw();
};
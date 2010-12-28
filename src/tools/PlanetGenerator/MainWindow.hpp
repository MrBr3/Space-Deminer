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

#include "./Model.hpp"

#include "SettingsWidget.hpp"

#include "SphereMesh.hpp"
#include "Texture.hpp"
#include "3DView.hpp"
#include "FileSettings.hpp"
#include "ViewSettings.hpp"
#include "LayerView.hpp"

const gint LENGTH_BORDER_WIDTH = 4;
const gint LENGTH_SMALLSPACE = 3;

/** \brief Represents the MainWindow of the Tool
 *
 * see Gtkmm Documentation for more information
 * */
class MainWindow : public Gtk::Window
{
  View3D* view_3d;
  ViewSettings* view_settings;

  class MyMenuItem : public Gtk::MenuItem
  {
  public:
    void set_accel_key(const Glib::ustring& key)
    {
      Gtk::MenuItem::set_accel_key(Gtk::AccelKey(key));
    }
  };
  class MyCheckMenuItem : public Gtk::CheckMenuItem
  {
  public:
    void set_accel_key(const Glib::ustring& key)
    {
      Gtk::CheckMenuItem::set_accel_key(Gtk::AccelKey(key));
    }
  };

  MyMenuItem menu_file;
  Gtk::Menu  menu_file_menu;
    MyMenuItem menu_file_quit;
  MyMenuItem menu_view;
  Gtk::Menu  menu_view_menu;
    MyCheckMenuItem menu_view_show_sidebar;
    MyCheckMenuItem menu_view_wireframed;
    MyMenuItem menu_view_settings;

  Gtk::HPaned _hpaned;
  Gtk::VPaned _vpaned;
  Gtk::VBox _vbox;
  Gtk::VBox _settings;
  Gtk::ScrolledWindow _layers_scrollbars;
  LayerView* _layers;
  Gtk::MenuBar _menu_bar;
  //Gtk::Toolbar _tool_bar;

  void draw_wireframed_toggled()
  {
    view_3d->set_draw_wireframed(menu_view_wireframed.get_active());
  }

  void toggle_show_sidebar()
  {
    set_show_sidebar(!get_show_sidebar());
  }
  bool get_show_sidebar()const{return menu_view_show_sidebar.get_active();}
  void set_show_sidebar(bool j)
  {
    menu_view_show_sidebar.set_active(j);
  }

  void _adapt_show_sidebar();

public:
  MainWindow();
  ~MainWindow()throw();

  void append_settings_widget(Gtk::Widget& w)
  {
    _settings.pack_start(w);
  }
};

/** \brief As long as the MainWindow is created, this pointer will point towards it
 *
 * \note NEVER! change this manually
 * */
extern MainWindow* main_window;
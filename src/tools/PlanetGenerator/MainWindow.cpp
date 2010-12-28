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

#include "MainWindow.hpp"

MainWindow* main_window  = nullptr;

MainWindow::MainWindow()
{
  g_assert(!main_window);
  main_window = this;

  _layers = Gtk::manage(new LayerView);
  view_3d = Gtk::manage(new View3D);
  view_settings = new ViewSettings;

  view_settings->signal_something_changed().connect(sigc::mem_fun(view_3d, &View3D::invalidate));
  view_settings->bring_to_front();

  add(_vbox);
    _vbox.show();
    _vbox.pack_start(_menu_bar, false, false);
      _menu_bar.show();
    //_vbox.pack_start(_tool_bar, false, false);
    //  _tool_bar.show();
    _vbox.pack_start(_hpaned);
      _hpaned.show();
      _hpaned.set_border_width(LENGTH_BORDER_WIDTH);
      _hpaned.pack1(*view_3d);
      _hpaned.pack2(_vpaned);
        _vpaned.show();
        _vpaned.pack1(_layers_scrollbars);
          _layers_scrollbars.show();
          _layers_scrollbars.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
          _layers_scrollbars.add(*_layers);
          _layers_scrollbars.set_shadow_type(Gtk::SHADOW_IN);
            _layers->show();
        _vpaned.pack2(_settings);
          _settings.show();
        _vpaned.set_position(96);

  _menu_bar.append(menu_file);
    menu_file.set_label(_("_File"));
    menu_file.set_use_underline();
    menu_file.set_submenu(menu_file_menu);
      menu_file_menu.append(menu_file_quit);
        menu_file_quit.set_accel_key("<control>q");
        menu_file_quit.set_label(_("_Quit"));
        menu_file_quit.set_use_underline();
        menu_file_quit.signal_activate().connect(sigc::ptr_fun(&Gtk::Main::quit));
  _menu_bar.append(menu_view);
    menu_view.set_label(_("_View"));
    menu_view.set_use_underline();
    menu_view.set_submenu(menu_view_menu);
      menu_view_menu.append(menu_view_show_sidebar);
        menu_view_show_sidebar.set_accel_key("F9");
        menu_view_show_sidebar.set_label(_("Show _Sidebar"));
        menu_view_show_sidebar.set_use_underline();
        menu_view_show_sidebar.signal_toggled().connect(sigc::mem_fun(*this, &MainWindow::_adapt_show_sidebar));
        menu_view_show_sidebar.set_active(true);
      menu_view_menu.append(menu_view_wireframed);
        menu_view_wireframed.set_accel_key("w");
        menu_view_wireframed.set_label(_("_Wireframed"));
        menu_view_wireframed.set_use_underline();
        menu_view_wireframed.set_active(view_3d->get_draw_wireframed());
        menu_view_wireframed.signal_toggled().connect(sigc::mem_fun(*this, &MainWindow::draw_wireframed_toggled));
        view_3d->signal_wireframed_changed().connect(sigc::mem_fun(menu_view_wireframed, &MyCheckMenuItem::set_active));
      menu_view_menu.append(menu_view_settings);
        menu_view_settings.set_label(_("View _Settings"));
        menu_view_settings.set_use_underline();
        menu_view_settings.signal_activate().connect(sigc::mem_fun(*view_settings, &SettingsWidget::bring_to_front));
  _menu_bar.show_all_children();

  set_default_size(480, 360);
  set_title(_("Planet Generator"));

  show();
}

MainWindow::~MainWindow()throw()
{
  view_3d->deinit();

  main_window = nullptr;
}

void MainWindow::_adapt_show_sidebar()
{
  g_assert(_hpaned.get_child2());

  int side_width  = _hpaned.get_child2()->get_width();
  bool maximized  = false;

  if(get_window())
    maximized = get_window()->get_state() & Gdk::WINDOW_STATE_MAXIMIZED;

  if(!get_show_sidebar())
  {
    _hpaned.get_child2()->hide();
    //_menu_bar.hide();
    //_tool_bar.hide();

    if(!maximized)
      resize(get_width()-side_width, get_height());
  }else
  {
    _hpaned.get_child2()->show();
    //_menu_bar.show();
    //_tool_bar.show();

    if(!maximized)
      resize(get_width()+side_width, get_height());
  }
}
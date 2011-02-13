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

#ifndef _SPACE_DEMINER_MAIN_WINDOW_H_
#define _SPACE_DEMINER_MAIN_WINDOW_H_

#include "./menu.hpp"

#include <gtkmm/window.h>
#include <gtkglmm.h>

#include "./framework/window.hpp"
#include "./framework/engine.hpp"

class MainWindow : public StaticManager<MainWindow, Gtk::Window>
{
public:
  Glib::RefPtr<MenuBack> menu_back;
  MainMenu main_menu_window;

  class GtkGlDrawingArea : public Gtk::GL::DrawingArea
  {
    bool _gl_initialized;

    Glib::RefPtr<Framework::Theme> _dark_theme;
  public:
    Framework::ResourceManager resource_manager;
    Glib::RefPtr<Framework::WindowManager> window_manager;

    const Glib::RefPtr<Framework::Theme>& get_dark_theme(){return _dark_theme;}
    const Glib::RefPtr<Framework::Theme>& get_default_theme(){return get_dark_theme();}

    void switch_debug_ui_mode();

    void on_realize();
    void on_size_allocate(Gtk::Allocation& allocation);
    bool on_expose_event(GdkEventExpose* event);

    void deinit();

    GtkGlDrawingArea();
    ~GtkGlDrawingArea();
  }gl_drawing_area;

  static const Glib::RefPtr<Framework::WindowManager>& get_window_manager(){return get_singleton()->gl_drawing_area.window_manager;}
    
  void init_ui();

public:
  void on_size_allocate(Gtk::Allocation& allocation);
  void on_show();
  void on_hide();

  bool on_key_release_event(GdkEventKey* e);

  MainWindow();
  ~MainWindow()throw();

private:
  Glib::RefPtr<Framework::Engine> _engine;
};

extern MainWindow* main_window;

#endif
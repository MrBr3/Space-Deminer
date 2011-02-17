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

#include "./ui/main-window.hpp"
#include <gtkmm/main.h>
#include <gdk/gdkkeysyms.h>

MainWindow* main_window  = nullptr;

void start_gui_test();

MainWindow::MainWindow()
{
  g_assert(!main_window);
  main_window = this;

  set_default_size(640, 480);

  add(gl_drawing_area);

  show_all_children();
}

MainWindow::~MainWindow()throw()
{
  menu_back.reset();

  gl_drawing_area.deinit();

  _engine.reset();

  g_assert(main_window==this);
  main_window = nullptr;
}

void MainWindow::init_ui()
{
  menu_back  = Glib::RefPtr<MenuBack>(new MenuBack());
  menu_back->register_window(Framework::WINDOW_LAYER_MENU_BACK);

  main_menu_window.register_window(Framework::WINDOW_LAYER_MENU_BACK);
  main_menu_window.show();

  sure_to_quit_dlg.setup_yes_no(_("Are you sure you wan't to quit Space-Deminer?"));
  sure_to_quit_dlg.register_window(Framework::WINDOW_LAYER_DIALOGS);
  sure_to_quit_dlg.signal_certain_response(Framework::Dialog::RESPONSE_YES).connect(sigc::ptr_fun(&Gtk::Main::quit));
}

bool MainWindow::on_key_release_event(GdkEventKey* e)
{
  switch(e->keyval)
  {
  case GDK_Tab:
    start_gui_test();
    return true;
  case GDK_F2:
    gl_drawing_area.switch_debug_ui_mode();
    return true;
  }
  return Window::on_key_release_event(e);
}

void MainWindow::on_show()
{
  Gtk::Window::on_show();

  if(Options::get_bool(OPTION_VIDEO_FULLSCREEN, true))
  {
    fullscreen();
  }
}

void MainWindow::on_hide()
{
  unfullscreen();

  Gtk::Window::on_hide();
}

void MainWindow::on_size_allocate(Gtk::Allocation& allocation)
{
  Glib::RefPtr<Gdk::Window> window  = get_window();

  if(window)
  {
    window->invalidate(true);
  }

  Gtk::Window::on_size_allocate(allocation);
}

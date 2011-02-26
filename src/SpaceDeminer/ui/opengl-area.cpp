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
#include "./ui/dark-theme.hpp"
#include <gtkmm/messagedialog.h>

MainWindow::GtkGlDrawingArea::GtkGlDrawingArea() : Gtk::GL::DrawingArea(Gdk::GL::Config::create(Gdk::GL::MODE_DEPTH|Gdk::GL::MODE_RGBA|Gdk::GL::MODE_DOUBLE|Gdk::GL::MODE_ALPHA|Gdk::GL::MODE_STENCIL)), window_manager(new Framework::WindowManager)
{
  Framework::Window::default_window_manager = window_manager.operator->();

  add_events(Gdk::BUTTON_PRESS_MASK|Gdk::BUTTON_RELEASE_MASK|Gdk::POINTER_MOTION_MASK|Gdk::LEAVE_NOTIFY_MASK|Gdk::ENTER_NOTIFY_MASK);

  _gl_initialized = false;
  window_manager->set_size(get_width(), get_height());

  signal_motion_notify_event().connect(sigc::mem_fun(*window_manager.operator->(), &Framework::WindowManager::handle_gtk_motion_notify_event));
  signal_leave_notify_event().connect(sigc::mem_fun(*window_manager.operator->(), &Framework::WindowManager::handle_gtk_leave_notify_event));
  signal_enter_notify_event().connect(sigc::mem_fun(*window_manager.operator->(), &Framework::WindowManager::handle_gtk_enter_notify_event));
  signal_button_press_event().connect(sigc::mem_fun(*window_manager.operator->(), &Framework::WindowManager::handle_gtk_button_press_event));
  signal_button_release_event().connect(sigc::mem_fun(*window_manager.operator->(), &Framework::WindowManager::handle_gtk_button_release_event));
}

MainWindow::GtkGlDrawingArea::~GtkGlDrawingArea()
{
}

void MainWindow::GtkGlDrawingArea::deinit()
{
  window_manager.reset();

  _dark_theme.reset();

  if(_gl_initialized)
  {
    Glib::RefPtr<Gdk::GL::Drawable> gl_drawable = get_gl_drawable();

    g_assert(gl_drawable);

    gl_drawable->gl_end();
  }
}

void MainWindow::GtkGlDrawingArea::on_realize()
{
  Gtk::GL::DrawingArea::on_realize();

  Glib::RefPtr<Gdk::GL::Drawable> gl_drawable = get_gl_drawable();

  g_assert(gl_drawable);

  gl_drawable->gl_begin(get_gl_context());

  GLenum err = glewInit();
  if(err!=GLEW_OK)
  {
    Gtk::MessageDialog dlg(_("couldn't load GLEW!"), false, Gtk::MESSAGE_ERROR);
    dlg.run();
    exit(-1);
  }
  if(!glewIsSupported("GL_VERSION_2_1"))
  {
    Gtk::MessageDialog dlg(_("OpenGL 2.1 not supported"), false, Gtk::MESSAGE_ERROR);
    dlg.set_secondary_text(_("Your Graphic Hardware or Driver does not support OpenGL 2.1, which is needed by this application!"));
    dlg.run();
    exit(-1);
  }

  MainWindow::get_singleton()->_engine = Framework::Engine::create_gl_engine(get_gl_context());

  _gl_initialized = true;

  _dark_theme = DarkTheme::create();
  _dark_theme->init();
  get_window_manager()->set_theme(_dark_theme);
  MainWindow::get_singleton()->init_ui();

  Glib::RefPtr<Gdk::Window> window = get_window();

  if(window)
    window->invalidate(false);
}

void MainWindow::GtkGlDrawingArea::on_size_allocate(Gtk::Allocation& allocation)
{
  Gtk::GL::DrawingArea::on_size_allocate(allocation);

  window_manager->set_size(allocation.get_width(), allocation.get_height());
}

bool MainWindow::GtkGlDrawingArea::on_expose_event(GdkEventExpose* event)
{
  if(!_gl_initialized)
      return false;

  Glib::RefPtr<Gdk::GL::Drawable> gl_drawable = get_gl_drawable();

  if(!gl_drawable)
    return false;

  /*if(only_part_invalidated) TODO
  {
  glReadBuffer(GL_FRONT);
  glDrawBuffer(GL_BACK);
  glCopyPixels(0, 0, get_width(), get_height(), GL_COLOR);
  }*/

  window_manager->expose();

  gl_drawable->swap_buffers();
  return true;
}

void MainWindow::GtkGlDrawingArea::switch_debug_ui_mode()
{
  using Framework::WindowManager;
  window_manager->set_debug_ui(WindowManager::DebugUI((window_manager->get_debug_ui()+1)%WindowManager::DEBUG_UI_N));
}

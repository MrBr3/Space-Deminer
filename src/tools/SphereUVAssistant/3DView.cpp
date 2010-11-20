/* Space Deminer is a small 2D Arcade Game. Your task ist to eliminate
 * intelligent Mines created in the last galactic war.
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

View3D::View3D() : Gtk::GL::DrawingArea(Gdk::GL::Config::create(Gdk::GL::MODE_RGBA|Gdk::GL::MODE_DOUBLE|Gdk::GL::MODE_ALPHA))
{
  add_events(Gdk::POINTER_MOTION_MASK|Gdk::LEAVE_NOTIFY_MASK|Gdk::ENTER_NOTIFY_MASK);

  show();

  set_size_request(320, 320);

  _gl_initialized = false;
}

View3D::~View3D()throw()
{
}

void View3D::deinit()
{
  if(_gl_initialized)
  {
    Glib::RefPtr<Gdk::GL::Drawable> gl_drawable = get_gl_drawable();

    g_assert(gl_drawable);

    gl_drawable->gl_end();
  }
}

void View3D::on_realize()
{
  Gtk::GL::DrawingArea::on_realize();

  Glib::RefPtr<Gdk::GL::Drawable> gl_drawable = get_gl_drawable();

  g_assert(gl_drawable);

  gl_drawable->gl_begin(get_gl_context());

  sphere_mesh.init();

  _gl_initialized = true;
}

bool View3D::on_expose_event(GdkEventExpose* event)
{
  if(!_gl_initialized)
    return false;

  Glib::RefPtr<Gdk::GL::Drawable> gl_drawable = get_gl_drawable();

  if(!gl_drawable)
    return false;

  gl_drawable->gl_begin(get_gl_context());

  glClearColor(view_settings->get_back_color_red(),
               view_settings->get_back_color_green(),
               view_settings->get_back_color_blue(),
               0.f);

  glClear(GL_COLOR_BUFFER_BIT);

  sphere_mesh.render();

  gl_drawable->swap_buffers();
  return true;
}
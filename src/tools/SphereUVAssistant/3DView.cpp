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

View3D::View3D() : Gtk::GL::DrawingArea(Gdk::GL::Config::create(Gdk::GL::MODE_RGBA|Gdk::GL::MODE_DOUBLE|Gdk::GL::MODE_ALPHA))
{
  add_events(Gdk::POINTER_MOTION_MASK|Gdk::BUTTON_PRESS_MASK|Gdk::BUTTON_RELEASE_MASK|Gdk::SCROLL_MASK|Gdk::LEAVE_NOTIFY_MASK|Gdk::ENTER_NOTIFY_MASK);

  set_size_request(320, 320);

  _rotating_with_mouse  = false;

  sphere  = Sphere::create();
  sphere->signal_invalidated().connect(sigc::mem_fun(*this, &View3D::invalidate));
  //sphere->rotate_x(30.f);
  //sphere->set_rotation_z_speed(10.f);

  show();
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

void View3D::invalidate()
{
  Glib::RefPtr<Gdk::Window> window  = get_window();

  if(!window)
    return;

  window->invalidate(true);
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

void View3D::on_size_allocate(Gtk::Allocation& allocation)
{
  Gtk::GL::DrawingArea::on_size_allocate(allocation);

  glViewport(0, 0, allocation.get_width(), allocation.get_height());
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

  glLoadIdentity();

  gfloat aspect = gfloat(get_width())/gfloat(get_height());
  if(aspect<=0.f)
    return false;
  if(aspect>=1.f)
    glOrtho(-aspect, aspect, 1.f, -1.f, sphere->get_scale()*1.001f, -sphere->get_scale()*1.001f);
  else
    glOrtho(-1.f, 1.f, 1.f/aspect, -1.f/aspect, sphere->get_scale()*1.001f, -sphere->get_scale()*1.001f);

  glRotatef(90.f, 1.f, 0.f, 0.f);
  glRotatef(sphere->get_x_rotation(), 1.f, 0.f, 0.f);
  glRotatef(sphere->get_z_rotation(), 0.f, 0.f, 1.f);

  glScalef(sphere->get_scale(), sphere->get_scale(), sphere->get_scale());

  if(sphere->get_draw_wireframed())
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glEnable(GL_CULL_FACE);

  sphere_mesh.render(sphere->get_use_warped_uv());

  gl_drawable->swap_buffers();
  return true;
}

bool View3D::on_scroll_event(GdkEventScroll* event)
{
  gfloat zoom = 0.f;

  if(event->direction==GDK_SCROLL_UP)
    zoom  = 1.f;
  else if(event->direction==GDK_SCROLL_DOWN)
    zoom  = -1.f;

  if(zoom==0.f)
    return false;

  sphere->set_scale(sphere->get_scale() + 0.1f*zoom);

  return true;
}

bool View3D::on_button_press_event(GdkEventButton* event)
{
  if(using_mouse())
    return false;

  mouse_drag_start_x  = event->x;
  mouse_drag_start_y  = event->y;

  if(event->button==2)
  _rotating_with_mouse  = true;
  return true;
}

bool View3D::on_button_release_event(GdkEventButton* event)
{
  if(!using_mouse())
    return false;

  _rotating_with_mouse  = false;
  return true;
}

bool View3D::on_motion_notify_event(GdkEventMotion* event)
{
  if(!using_mouse())
    return false;

  if(_rotating_with_mouse)
  {
    sphere->rotate_z(mouse_drag_start_x-event->x*1.f);
    sphere->rotate_x(mouse_drag_start_y-event->y*1.f);

    mouse_drag_start_x  = event->x;
    mouse_drag_start_y  = event->y;
  }

  return true;
}
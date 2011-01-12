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

View3D::View3D() : Gtk::GL::DrawingArea(Gdk::GL::Config::create(Gdk::GL::MODE_DEPTH|Gdk::GL::MODE_RGBA|Gdk::GL::MODE_DOUBLE|Gdk::GL::MODE_ALPHA))
{
  add_events(Gdk::POINTER_MOTION_MASK|Gdk::BUTTON_PRESS_MASK|Gdk::BUTTON_RELEASE_MASK|Gdk::SCROLL_MASK|Gdk::LEAVE_NOTIFY_MASK|Gdk::ENTER_NOTIFY_MASK);

  set_size_request(320, 320);

  _rotating_with_mouse  = false;
  _draw_wireframed  = false;

  planet  = Planet::create();
  planet->signal_invalidated().connect(sigc::mem_fun(*this, &View3D::invalidate));

  distance  = 1.f;

  show();
  _gl_initialized = false;

  LayerModel::signal_something_changed().connect(sigc::mem_fun(*this, &View3D::invalidate));

  base_texture  = Texture::create(BaseTextureLayer::get_imagefile());
  cloud_texture  = Texture::create(CloudTextureLayer::get_imagefile());
  night_texture  = Texture::create(NightTextureLayer::get_imagefile());
  weight_texture  = Texture::create(WeightTextureLayer::get_imagefile());
  ring_texture  = Texture::create(RingLayer::get_imagefile());
}

View3D::~View3D()throw()
{
}

void View3D::unbind_all_textures()
{
  base_texture->unbind();
  cloud_texture->unbind();
  night_texture->unbind();
  weight_texture->unbind();
  ring_texture->unbind();
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

  sphere_mesh.init(view_settings->get_n_sphere_segments());
  ring_mesh.init(view_settings->get_n_ring_segments());
  base_texture->init();
  cloud_texture->init();
  night_texture->init();
  weight_texture->init();
  ring_texture->init();
  ring_texture->set_wrapping(Texture::CLAMP, Texture::REPEAT);

  glEnable(GL_DEPTH_TEST);

  glDepthFunc(GL_LEQUAL);
  glDepthMask(GL_TRUE);

  _gl_initialized = true;

  view_settings->signal_n_sphere_segments_changed().connect(sigc::mem_fun(*this, &View3D::reinit_sphere_mesh));
  view_settings->signal_n_ring_segments_changed().connect(sigc::mem_fun(*this, &View3D::reinit_ring_mesh));

  signal_wireframed_changed().connect(sigc::hide(sigc::mem_fun(sig_wireframed_changed_noparam(), &sigc::signal<void>::emit)));
}

void View3D::reinit_sphere_mesh()
{
  sphere_mesh.set_segment_division(view_settings->get_n_sphere_segments());
  invalidate();
}

void View3D::reinit_ring_mesh()
{
  ring_mesh.set_segment_division(view_settings->get_n_ring_segments());
  invalidate();
}

void View3D::on_size_allocate(Gtk::Allocation& allocation)
{
  Gtk::GL::DrawingArea::on_size_allocate(allocation);

  glViewport(0, 0, allocation.get_width(), allocation.get_height());
}

Matrix44 View3D::calc_projection_matrix(Matrix44& dest, gfloat aspect)const
{
  dest.set_perspective(50., aspect, 1.e-2f, 1000.f);
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
  glClearDepth(1.f);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_CULL_FACE);
  glDisable(GL_BLEND);

  if(get_draw_wireframed())
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glMatrixMode(GL_TEXTURE);

  glLoadIdentity();

  glMatrixMode(GL_PROJECTION);

  gfloat aspect = gfloat(get_width())/gfloat(get_height());

  calc_projection_matrix(projection_matrix, aspect);
  projection_matrix.glLoadMatrix();

  glMatrixMode(GL_MODELVIEW);

  view_matrix.set_translate(0., 0.f, -1.f -1e-2f - 1.5f*distance);
  view_matrix.rotate_x(-90.f);
  view_matrix.glLoadMatrix();


  glPushMatrix();

  planet_model_matrix.set_rotate_x(planet->get_x_rotation());
  planet_model_matrix.rotate_z(planet->get_z_rotation());
  planet_model_matrix.glMultMatrix();

  bool warped_uv = false;

  glEnable(GL_TEXTURE_2D);
  Glib::RefPtr<Layer> only_visible_layer  = LayerModel::just_one_layer_visible(); // Gets the only visible Layer
  if(only_visible_layer.operator->()==BaseTextureLayer::get_singleton())
  {
    base_texture->bind();
    warped_uv = BaseTextureLayer::get_imagefile()->get_needs_to_be_warped();
  }else if(only_visible_layer.operator->()==CloudTextureLayer::get_singleton())
  {
    cloud_texture->bind();
    warped_uv = CloudTextureLayer::get_imagefile()->get_needs_to_be_warped();
  }else if(only_visible_layer.operator->()==NightTextureLayer::get_singleton())
  {
    night_texture->bind();
    warped_uv = NightTextureLayer::get_imagefile()->get_needs_to_be_warped();
  }else if(only_visible_layer.operator->()==WeightTextureLayer::get_singleton())
  {
    weight_texture->bind();
    warped_uv = WeightTextureLayer::get_imagefile()->get_needs_to_be_warped();
  }else if(only_visible_layer.operator->()!=RingLayer::get_singleton())
  {
    base_texture->bind();
    warped_uv = BaseTextureLayer::get_imagefile()->get_needs_to_be_warped();
  }

  sphere_mesh.render(warped_uv);

  unbind_all_textures();

  glPopMatrix();

  if(RingLayer::get_singleton()->get_visible())
  {
    RingLayer* ring_planet = RingLayer::get_singleton();

    glPushMatrix();

    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ring_model_matrix = planet_model_matrix;
    ring_model_matrix.rotate_z(ring_planet->get_z_rotation());
    ring_model_matrix.rotate_x(ring_planet->get_x_rotation());
    ring_model_matrix.glMultMatrix();

    ring_texture->bind();

    ring_mesh.render(ring_planet->get_width(), ring_planet->get_outer_radius());

    unbind_all_textures();

    glPopMatrix();
  }

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

  distance  = CLAMP(distance-0.1f*zoom*distance, 0.f, 32.f);
  invalidate();

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
    gfloat r_frac = CLAMP(distance*distance*1.5f, 0.1f, 1.f);

    planet->rotate_z(-(mouse_drag_start_x-event->x*1.f) * r_frac);
    planet->rotate_x(-(mouse_drag_start_y-event->y*1.f) * r_frac);

    mouse_drag_start_x  = event->x;
    mouse_drag_start_y  = event->y;
  }

  return true;
}
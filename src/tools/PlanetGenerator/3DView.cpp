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

const Matrix44& get_ring_model_matrix()
{
  if(!main_window->get_view_3d())
    return Matrix44::identity;
  else
    return main_window->get_view_3dA().ring_model_matrix;
}
const Matrix44& get_planet_model_matrix()
{
  if(!main_window->get_view_3d())
    return Matrix44::identity;
  else
    return main_window->get_view_3dA().planet_model_matrix;
}

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

  g_assert(RingLayer::get_singleton());
  for(gsize i=0; i<N_LIGHT_LAYERS; ++i)
  {
    g_assert(LightLayer::get_singleton(i));
    RingLayer::get_singleton()->signal_rotation_changed().connect(sigc::mem_fun(*LightLayer::get_singleton(i), &LightLayer::recalc_pos));
    LightLayer::get_singleton(i)->recalc_pos();
  }

  base_texture  = Texture::create(BaseTextureLayer::get_imagefile());
  cloud_texture  = Texture::create(CloudTextureLayer::get_imagefile());
  night_texture  = Texture::create(NightTextureLayer::get_imagefile());
  weight_texture  = Texture::create(WeightTextureLayer::get_imagefile());
  ring_texture  = Texture::create(RingLayer::get_imagefile());

  planet_program = ring_program = dummy_program = 0;
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
    deinit_shaders();

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

  GLenum err = glewInit();
  if(err!=GLEW_OK)
  {
    Gtk::MessageDialog dlg(_("couldn't load GLEW!"), false, Gtk::MESSAGE_ERROR);
    dlg.run();
    exit(-1);
  }
  if(!glewIsSupported("GL_VERSION_3_3"))
  {
    Gtk::MessageDialog dlg(_("OpenGL 2.1 not supported"), false, Gtk::MESSAGE_ERROR);
    dlg.set_secondary_text(_("Your Graphic Hardware or Driver does not support OpenGL 3.3, which is needed by this application!"));
    dlg.run();
    exit(-1);
  }

  init_shaders();

  sphere_mesh.init(view_settings->get_n_sphere_segments());
  ring_mesh.init(view_settings->get_n_ring_segments());
  base_texture->init();
  cloud_texture->init();
  night_texture->init();
  weight_texture->init();
  ring_texture->init();
  ring_texture->set_wrapping(Texture::CLAMP, Texture::REPEAT);
  lightsource_mesh.init();

  glEnable(GL_DEPTH_TEST); //CHECK

  glDepthFunc(GL_LEQUAL); //CHECK
  glDepthMask(GL_TRUE); //CHECK

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

  glViewport(0, 0, allocation.get_width(), allocation.get_height()); //CHECK
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

  glUseProgram(planet_program);

  matrix_stack.clear();

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

  //glMatrixMode(GL_TEXTURE); //REMOVE
  //glLoadIdentity(); //REMOVE

  //glMatrixMode(GL_PROJECTION); //REMOVE

  gfloat aspect = gfloat(get_width())/gfloat(get_height());

  calc_projection_matrix(projection_matrix, aspect);
  matrix_stack.top() = projection_matrix;

  view_matrix.set_translate(0., 0.f, -1.f -1e-2f - 1.5f*distance);
  view_matrix.rotate_x(-90.f);
  view_matrix.rotate_x(planet->get_x_rotation());
  view_matrix.rotate_z(planet->get_z_rotation());
  matrix_stack.top() *= view_matrix;
  matrix_stack.top().glUniform(planet_program_uniform.matrix_PV);

  //glMatrixMode(GL_MODELVIEW); //REMOVE

  matrix_stack.push(false);

  matrix_stack.top() *= planet_model_matrix;
  //matrix_stack.top().glLoadMatrix(); //REMOVE
  matrix_stack.top().glUniform(planet_program_uniform.matrix_M);

  bool warped_uv = false;

  //glEnable(GL_TEXTURE_2D); //REMOVE
  Glib::RefPtr<Layer> only_visible_texture_layer  = LayerModel::just_one_texture_layer_visible(); // Gets the only visible Layer
  if(only_visible_texture_layer.operator->()==CloudTextureLayer::get_singleton())
  {
    cloud_texture->bind();
    warped_uv = CloudTextureLayer::get_imagefile()->get_needs_to_be_warped();
  }else if(only_visible_texture_layer.operator->()==NightTextureLayer::get_singleton())
  {
    night_texture->bind();
    warped_uv = NightTextureLayer::get_imagefile()->get_needs_to_be_warped();
  }else if(only_visible_texture_layer.operator->()==WeightTextureLayer::get_singleton())
  {
    weight_texture->bind();
    warped_uv = WeightTextureLayer::get_imagefile()->get_needs_to_be_warped();
  }else
  {
    base_texture->bind();
    warped_uv = BaseTextureLayer::get_imagefile()->get_needs_to_be_warped();
  }

  sphere_mesh.render(warped_uv);

  unbind_all_textures();

  matrix_stack.pop();

  if(RingLayer::get_singleton()->get_visible())
  {
    RingLayer* ring_planet = RingLayer::get_singleton();

    matrix_stack.push(false);

    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ring_model_matrix = planet_model_matrix;
    ring_model_matrix.rotate_z(ring_planet->get_z_rotation());
    ring_model_matrix.rotate_x(ring_planet->get_x_rotation());
    matrix_stack.top() *= ring_model_matrix;
    matrix_stack.top().glLoadMatrix();

    ring_texture->bind();

    ring_mesh.render(ring_planet->get_width(), ring_planet->get_outer_radius());

    unbind_all_textures();

    matrix_stack.pop();
  }

  for(gsize i=0; i<N_LIGHT_LAYERS; ++i)
  {
    const LightLayer& light_layer = *LightLayer::get_singleton(i);
    g_assert(&light_layer);

    if(light_layer.get_light_type()==LightLayer::LIGHT_TYPE_AMBIENT)
      continue;

    matrix_stack.push(false);
    /*glPushMatrix(); //REMOVE
      glTranslatef(light_layer.position.x, light_layer.position.y, light_layer.position.z);; //REMOVE
      lightsource_mesh.point_mesh.RenderBatch();
    glPopMatrix();*/ //REMOVE
    matrix_stack.pop();
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

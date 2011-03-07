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

  _draw_light_representation = false;

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

  planet_program = ring_program = simple_program = 0;
}

View3D::~View3D()throw()
{
}

void View3D::unbind_all_textures()
{
  Texture::unbind(0);
  Texture::unbind(1);
  Texture::unbind(2);
  Texture::unbind(3);
  CurveTexture::unbind(/*4*/);
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
  GLint n_texture_stages;
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &n_texture_stages);

  if(n_texture_stages<ENSURED_N_TEXTURE_STAGES)
  {
    Gtk::MessageDialog dlg(_("Too less Texture stages supported"), false, Gtk::MESSAGE_ERROR);
    dlg.set_secondary_text(Glib::ustring::compose(_("Your Graphic Hardware or Driver does only support %1 Texture stages.\nThis application needs %2!"), n_texture_stages, ENSURED_N_TEXTURE_STAGES));
    dlg.run();
    exit(-1);
  }

  GLint n_fragment_uniform_stages, n_vertex_uniform_stages, n_min_uniform_stages;
  glGetIntegerv(GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS, &n_fragment_uniform_stages);
  glGetIntegerv(GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS, &n_vertex_uniform_stages);

  n_min_uniform_stages = MIN(n_vertex_uniform_stages, n_fragment_uniform_stages);

  if(n_min_uniform_stages<ENSURED_N_UNIFORM_STAGES)
  {
    Gtk::MessageDialog dlg(_("Too less Uniform stages supported"), false, Gtk::MESSAGE_ERROR);
    dlg.set_secondary_text(Glib::ustring::compose(_("Your Graphic Hardware or Driver does only support %1 Uniform stages\nThis application needs %2!"), n_min_uniform_stages, ENSURED_N_UNIFORM_STAGES));
    dlg.run();
    exit(-1);
  }

  init_shaders();

  sphere_mesh.init(view_settings->get_n_sphere_segments());
  ring_mesh.init(view_settings->get_n_ring_segments());
  ring_mesh.ring_uv_factor = glGetUniformLocation(ring_program, "ring_uv_factor");
  base_texture->init();
  cloud_texture->init();
  night_texture->init();
  weight_texture->init();
  ring_texture->init();
  ring_texture->set_wrapping(Texture::CLAMP, Texture::REPEAT);
  lightsource_mesh.init();

  glEnable(GL_DEPTH_TEST);

  glDepthFunc(GL_LEQUAL);
  glDepthMask(GL_TRUE);

  _gl_initialized = true;

  view_settings->signal_n_sphere_segments_changed().connect(sigc::mem_fun(*this, &View3D::reinit_sphere_mesh));
  view_settings->signal_n_ring_segments_changed().connect(sigc::mem_fun(*this, &View3D::reinit_ring_mesh));

  signal_wireframed_changed().connect(sigc::hide(sigc::mem_fun(sig_wireframed_changed_noparam(), &sigc::signal<void>::emit)));
  signal_draw_light_representation_changed().connect(sigc::hide(sigc::mem_fun(sig_draw_light_representation_changed_noparam(), &sigc::signal<void>::emit)));
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

  Matrix44 matrix_PV, matrix_M;

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

  gfloat aspect = gfloat(get_width())/gfloat(get_height());

  calc_projection_matrix(projection_matrix, aspect);
  matrix_stack.top() = projection_matrix;

  view_matrix.set_translate(0., 0.f, -1.f -1e-2f - 1.5f*distance);
  view_matrix.rotate_x(-90.f);
  view_matrix.rotate_x(planet->get_x_rotation());
  view_matrix.rotate_z(planet->get_z_rotation());
  matrix_stack.top() *= view_matrix;
  matrix_PV = matrix_stack.top();

  Vector3 camera_pos = view_matrix.get_inversion()*Vector3(0.f, 0.f, 0.f);
  Vector3 planet_pos = planet_model_matrix*Vector3(0.f, 0.f, 0.f);

  matrix_stack.push(false);

  matrix_stack.top() *= planet_model_matrix;
  matrix_M = matrix_stack.top();

  glUseProgram(planet_program);

  bool no_lights = true;
  bool no_nighttexture = true;
  for(gsize i=0; i<N_LIGHT_LAYERS; ++i)
  {
    const LightLayer& light_layer = *LightLayer::get_singleton(i);
    g_assert(&light_layer);

    if(!light_layer.get_visible())
      continue;

    no_lights = false;

    if(light_layer.get_influence_night() > 0.f)
      no_nighttexture = false;
  }
  glUniform1i(planet_program_uniform.uni_no_lightning, no_lights);
  glUniform1i(planet_program_uniform.uni_no_nighttexture, no_nighttexture||!NightTextureLayer::get_singleton()->get_visible());

  glUniform1i(planet_program_uniform.uni_just_one_texture_visible, LayerModel::just_one_texture_layer_visible() && !BaseTextureLayer::get_singleton()->get_visible());
  glUniform1i(planet_program_uniform.uni_base_texture_visible, BaseTextureLayer::get_singleton()->get_visible());
  glUniform1i(planet_program_uniform.uni_base_texture_warped, BaseTextureLayer::get_singleton()->get_imagefile()->get_needs_to_be_warped());
  planet_program_uniform.uni_base_texture_colorcurves.feed_data(BaseTextureLayer::get_singleton()->get_imagefile()->get_color_curve());
  glUniform1i(planet_program_uniform.uni_night_texture_visible, NightTextureLayer::get_singleton()->get_visible());
  glUniform1i(planet_program_uniform.uni_night_texture_warped, NightTextureLayer::get_singleton()->get_imagefile()->get_needs_to_be_warped());
  planet_program_uniform.uni_night_texture_colorcurves.feed_data(NightTextureLayer::get_singleton()->get_imagefile()->get_color_curve());
  glUniform1i(planet_program_uniform.uni_cloud_texture_visible, CloudTextureLayer::get_singleton()->get_visible());
  glUniform1i(planet_program_uniform.uni_cloud_texture_warped, CloudTextureLayer::get_singleton()->get_imagefile()->get_needs_to_be_warped());
  planet_program_uniform.uni_cloud_texture_curve.feed_data(CloudTextureLayer::get_singleton()->get_imagefile()->get_contrast_curve());
  glUniform1i(planet_program_uniform.uni_weight_texture_visible, WeightTextureLayer::get_singleton()->get_visible());
  glUniform1i(planet_program_uniform.uni_weight_texture_warped, WeightTextureLayer::get_singleton()->get_imagefile()->get_needs_to_be_warped());
  planet_program_uniform.uni_weight_texture_colorcurves.feed_data(WeightTextureLayer::get_singleton()->get_imagefile()->get_color_curve());

  base_texture->bind(0, BaseTextureLayer::get_singleton()->get_visible());
  night_texture->bind(1, NightTextureLayer::get_singleton()->get_visible());
  cloud_texture->bind(2, CloudTextureLayer::get_singleton()->get_visible());
  weight_texture->bind(3, WeightTextureLayer::get_singleton()->get_visible());
  CurveTexture::bind(/*4*/);

  glUniform1i(planet_program_uniform.base_texture, 0);
  glUniform1i(planet_program_uniform.night_texture, 1);
  glUniform1i(planet_program_uniform.cloud_texture, 2);
  glUniform1i(planet_program_uniform.weight_texture, 3);
  glUniform1i(planet_program_uniform.uni_all_curves, 4);

  matrix_PV.glUniform(planet_program_uniform.matrix_PV);
  matrix_M.glUniform(planet_program_uniform.matrix_M);

  planet_program_uniform.light[0].feed_data(0);
  planet_program_uniform.light[1].feed_data(1);
  planet_program_uniform.light[2].feed_data(2);
  planet_program_uniform.light[3].feed_data(3);

  planet_program_uniform.night_gradient.feed_data(NightTextureLayer::get_singleton()->get_night_gradient());
  glUniform1i(planet_program_uniform.uni_night_gradient_depends_on_diffuse, NightTextureLayer::get_singleton()->get_diffuse_depending());

  planet_program_uniform.cloud_gradient.feed_data(CloudTextureLayer::get_singleton()->get_cloud_gradient());

  sphere_mesh.render();

  unbind_all_textures();

  matrix_stack.pop();

  if(_draw_light_representation)
  {
    glUseProgram(simple_program);

    glEnableVertexAttribArray(1);
    for(gsize i=0; i<N_LIGHT_LAYERS; ++i)
    {
      const LightLayer& light_layer = *LightLayer::get_singleton(i);
      g_assert(&light_layer);

      if(!light_layer.get_visible())
        continue;

      if(light_layer.get_light_type()==LightLayer::LIGHT_TYPE_POINT)
      {
        matrix_stack.push(false);
          matrix_stack.top().translate(light_layer.position);

          matrix_M = matrix_stack.top();

          matrix_PV.glUniform(simple_program_uniform.matrix_PV);
          matrix_M.glUniform(simple_program_uniform.matrix_M);
          lightsource_mesh.point_mesh.RenderBatch();
        matrix_stack.pop();
      }else if(light_layer.get_light_type()==LightLayer::LIGHT_TYPE_DIRECTIONAL || light_layer.get_light_type()==LightLayer::LIGHT_TYPE_CUSTOM)
      {
        matrix_stack.push(false);
          matrix_stack.top().translate(-light_layer.direction*3.f);
          matrix_stack.top().rotate_z(light_layer.get_z_rotation());
          matrix_stack.top().rotate_x(light_layer.get_x_rotation());

          matrix_M = matrix_stack.top();

          matrix_PV.glUniform(simple_program_uniform.matrix_PV);
          matrix_M.glUniform(simple_program_uniform.matrix_M);
          lightsource_mesh.point_mesh.RenderBatch();
          lightsource_mesh.directional_light.RenderBatch();
        matrix_stack.pop();
      }
    }
    glDisableVertexAttribArray(1);
  }

  if(RingLayer::get_singleton()->get_visible())
  {
    glUseProgram(ring_program);

    RingLayer* ring_planet = RingLayer::get_singleton();

    matrix_stack.push(false);

    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ring_model_matrix.set_identity();
    ring_model_matrix.rotate_z(ring_planet->get_z_rotation());
    ring_model_matrix.rotate_x(ring_planet->get_x_rotation());
    matrix_stack.top() = ring_model_matrix;
    matrix_stack.top().scale(ring_planet->get_outer_radius());
    matrix_M = matrix_stack.top();

    Vector3 normal = (matrix_M*Vector4(0.f, 0.f, 1.f, 0.f)).get_xyz();
    normal.normalize();

    {
      Plane ring_plane;
      ring_plane.set(normal, planet_pos);
      if(ring_plane.check_point(camera_pos)==Math::BACKSIDE)
        normal = -normal;
    }

    normal.glUniform4(ring_program_uniform.uni_ring_normal, 0.f);

    matrix_PV.glUniform(ring_program_uniform.matrix_PV);
    matrix_M.glUniform(ring_program_uniform.matrix_M);

    ring_program_uniform.light[0].feed_data(0);
    ring_program_uniform.light[1].feed_data(1);
    ring_program_uniform.light[2].feed_data(2);
    ring_program_uniform.light[3].feed_data(3);

    ring_program_uniform.uni_ring_texture_colorcurves.feed_data(RingLayer::get_singleton()->get_imagefile()->get_color_curve());

    glUniform1i(ring_program_uniform.uni_no_lightning, no_lights);

    glActiveTexture(GL_TEXTURE0);
    ring_texture->bind(0);
    CurveTexture::bind(/*4*/);

    glUniform1i(ring_program_uniform.ring_texture, 0);
    glUniform1i(ring_program_uniform.uni_all_curves, 4);

    ring_mesh.render(ring_planet->get_width(), ring_planet->get_outer_radius());

    unbind_all_textures();

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

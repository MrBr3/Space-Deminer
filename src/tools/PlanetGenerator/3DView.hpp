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

 const gsize ENSURED_N_UNIFORM_STAGES = 512;

class View3D : public Gtk::GL::DrawingArea
{
  bool _gl_initialized;
  bool _draw_wireframed;

  bool _rotating_with_mouse;
  bool using_mouse()const throw()
  {
    return _rotating_with_mouse;
  }
  int mouse_drag_start_x, mouse_drag_start_y;

  gfloat distance;

  sigc::signal<void, bool> _signal_wireframed_changed;
  sigc::signal<void> _sig_wireframed_changed_noparam;

  void reinit_sphere_mesh();
  void reinit_ring_mesh();

  Matrix44Stack matrix_stack;

public:

  View3D();
  ~View3D()throw();

  void deinit();

  void on_realize();
  bool on_expose_event(GdkEventExpose* event);
  void on_size_allocate(Gtk::Allocation& allocation);

  bool on_scroll_event(GdkEventScroll* event);
  bool on_button_press_event(GdkEventButton* event);
  bool on_button_release_event(GdkEventButton* event);
  bool on_motion_notify_event(GdkEventMotion* event);

  void invalidate();

  Glib::RefPtr<Planet> planet;

  /** \brief Gets, whether the Mesh should be drawn wireframed.
   * */
  bool get_draw_wireframed()const{return _draw_wireframed;}

  /** \brief Sets, whether the Mesh should be drawn wireframed.
   *
   * */
  void set_draw_wireframed(bool draw_wireframed=true)
  {
    if(_draw_wireframed==draw_wireframed)
      return;

    _draw_wireframed  = draw_wireframed;
    _signal_wireframed_changed.emit(draw_wireframed);
    invalidate();
  }

  sigc::signal<void, bool>& signal_wireframed_changed(){return _signal_wireframed_changed;}
  sigc::signal<void>& sig_wireframed_changed_noparam(){return _sig_wireframed_changed_noparam;}

  SphereMesh sphere_mesh;
  RingMesh ring_mesh;
  LightSourceMesh lightsource_mesh;
  Glib::RefPtr<Texture> base_texture, cloud_texture, night_texture, weight_texture, ring_texture;

  void unbind_all_textures();

  Matrix44 calc_projection_matrix(Matrix44& dest, gfloat aspect)const;

  Matrix44 planet_model_matrix, ring_model_matrix, view_matrix, projection_matrix;

// Shaders
  struct GradientUniform
  {
    GLuint curves;
    GLuint defcolor, col[4];
    GLuint remap[2];

    void get_uniform_locations(GLuint program, const std::string& prefix);
    void feed_data(const ConstGradientPtr& gradient);
  };
  struct PlanetProgramUniform
  {
    GLuint matrix_PV;
    GLuint matrix_M;
    GLuint uni_texture_layer_visibility;
    GLuint base_texture;
    GLuint night_texture;
    GLuint cloud_texture;
    GLuint weight_texture;
    GLuint uni_just_one_texture_visible;
    GLuint uni_base_texture_visible;
    GLuint uni_base_texture_warped;
    GLuint uni_night_texture_visible;
    GLuint uni_night_texture_warped;
    GLuint uni_cloud_texture_visible;
    GLuint uni_cloud_texture_warped;
    GLuint uni_weight_texture_visible;
    GLuint uni_weight_texture_warped;
    GLuint no_lightning;

    struct Light
    {
      struct GradientLight
      {
        GLuint use;
        GLuint multiply_gradient_color_with_light_color;
        //GLuint add_x_rotation, add_z_rotation;
        //GLuint radius;
        GLuint inside_planet, outside_planet;
        GLuint modulate_type;
        GradientUniform light_gradient;

        void get_uniform_locations(GLuint program, const std::string& prefix);
        void feed_data(const LightLayer::GradientSetting& gradient);
      };

      GLuint visible, type;
      GLuint dir, pos, color;
      GLuint influence_night, light_on_planet, light_on_ring;
      GLuint specular_factor, ring_shadow, cloud_shadow;
      GradientUniform shade_gradient;
      GradientLight gradient[4];

      void get_uniform_locations(GLuint program, const std::string& prefix);
      void feed_data(guint i);
    };

    Light light[4];
  }planet_program_uniform;
  struct RingProgramUniform
  {
    GLuint matrix_PV;
    GLuint matrix_M;
    GLuint ring_texture;
  }ring_program_uniform;
  struct SimpleProgramUniform
  {
    GLuint matrix_PV;
    GLuint matrix_M;
  }simple_program_uniform;
  GLuint planet_program, ring_program, simple_program;
  void init_shaders();
  void deinit_shaders();
};

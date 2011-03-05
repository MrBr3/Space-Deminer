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

enum BadShader
{
  BAD_SHADER
};

class View3D : public Gtk::GL::DrawingArea
{
  bool _gl_initialized;
  bool _draw_wireframed;

  bool _draw_light_representation;

  bool _rotating_with_mouse;
  bool using_mouse()const throw()
  {
    return _rotating_with_mouse;
  }
  int mouse_drag_start_x, mouse_drag_start_y;

  gfloat distance;

  sigc::signal<void, bool> _signal_wireframed_changed;
  sigc::signal<void> _sig_wireframed_changed_noparam;
  sigc::signal<void, bool> _signal_draw_light_representation_changed;
  sigc::signal<void> _sig_draw_light_representation_changed_noparam;

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
  bool get_draw_light_representation()const{return _draw_light_representation;}

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

  void set_draw_light_representation(bool lr)
  {
    if(_draw_light_representation==lr)
      return;

    _draw_light_representation = lr;
    _signal_draw_light_representation_changed.emit(_draw_light_representation);
    invalidate();
  }

  sigc::signal<void, bool>& signal_wireframed_changed(){return _signal_wireframed_changed;}
  sigc::signal<void>& sig_wireframed_changed_noparam(){return _sig_wireframed_changed_noparam;}

  sigc::signal<void, bool>& signal_draw_light_representation_changed(){return _signal_draw_light_representation_changed;}
  sigc::signal<void>& sig_draw_light_representation_changed_noparam(){return _sig_draw_light_representation_changed_noparam;}

  SphereMesh sphere_mesh;
  RingMesh ring_mesh;
  LightSourceMesh lightsource_mesh;
  Glib::RefPtr<Texture> base_texture, cloud_texture, night_texture, weight_texture, ring_texture;

  void unbind_all_textures();

  Matrix44 calc_projection_matrix(Matrix44& dest, gfloat aspect)const;

  Matrix44 planet_model_matrix, ring_model_matrix, view_matrix, projection_matrix;

  class CurveTexture : public sigc::trackable
  {
    static GLuint next_texture_slize_to_create;
    static GLuint texture_id;
    static gsize n_referenced;
    static GLfloat* samples;

    int this_slice;

    ConstGradientPtr gradient;
    ConstCurvePtr curve;

    void fill_texture();
    void init();
  public:
    void set(GradientPtr g);
    void set(CurvePtr g);
    GLfloat get_slice_id()const{return this_slice;}

    static void bind();
    static void unbind();

    CurveTexture();
    ~CurveTexture()throw();
  };

// Shaders
  struct GradientUniform
  {
    GLuint slice_id;
    GLuint remap_offset, remap_size;

    CurveTexture curves_texture;

    void get_uniform_locations(GLuint program, const std::string& prefix);
    void feed_data(const GradientPtr& gradient);
  };
  struct CurveUniform
  {
    GLuint slice_id;

    CurveTexture curves_texture;

    void get_uniform_locations(GLuint program, const std::string& prefix);
    void feed_data(const CurvePtr& gradient);
  };
  struct PlanetProgramUniform
  {
    GLint matrix_PV;
    GLint matrix_M;
    GLint uni_texture_layer_visibility;
    GLint base_texture;
    GLint night_texture;
    GLint cloud_texture;
    GLint weight_texture;
    GLint uni_just_one_texture_visible;
    GLint uni_base_texture_visible;
    GLint uni_base_texture_warped;
    GLint uni_night_texture_visible;
    GLint uni_night_texture_warped;
    GLint uni_night_gradient_depends_on_diffuse;
    GLint uni_cloud_texture_visible;
    GLint uni_cloud_texture_warped;
    GLint uni_weight_texture_visible;
    GLint uni_weight_texture_warped;
    GLint uni_no_lightning;
    GLint uni_no_nighttexture;
    GLint uni_all_curves;

    GradientUniform night_gradient;
    GradientUniform cloud_gradient;

    struct Light
    {
      struct GradientLight
      {
        GLint use;
        GLint multiply_gradient_color_with_light_color;
        //GLint add_x_rotation, add_z_rotation;
        //GLint radius;
        GLint inside_planet;//, outside_planet;
        GLint modulate_type;
        GradientUniform light_gradient;

        void get_uniform_locations(GLuint program, const std::string& prefix);
        void feed_data(const LightLayer::GradientSetting& gradient);
      };

      GLint visible, type;
      GLint dir, pos, color;
      GLint influence_night, light_on_planet;//, light_on_ring;
      GLint specular_factor, ring_shadow, cloud_shadow;//planet shadow
      GLint just_shadows;
      GradientUniform shade_gradient;
      CurveUniform planet_shade_gradient;
      GradientLight gradient[4];

      void get_uniform_locations(GLuint program, const std::string& prefix);
      void feed_data(guint i);
    };

    Light light[4];
  }planet_program_uniform;
  struct RingProgramUniform
  {
    GLint matrix_PV;
    GLint matrix_M;
    GLint ring_texture;
  }ring_program_uniform;
  struct SimpleProgramUniform
  {
    GLint matrix_PV;
    GLint matrix_M;
  }simple_program_uniform;
  GLuint planet_program, ring_program, simple_program;
  void init_shaders();
  void deinit_shaders();
};

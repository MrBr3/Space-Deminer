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

const gsize N_LIGHT_LAYERS = 4;

class LightLayer : public MultiLayer<LightLayer>
{
public:
  typedef ImageLayer<LightLayer> ParentClass;

  enum LightType
  {
    LIGHT_TYPE_AMBIENT = 0,
    LIGHT_TYPE_DIRECTIONAL = 1,
    LIGHT_TYPE_POINT = 2,
  };

  enum GradientModulateType
  {
    GRADIENT_MODULATE_NORMAL=0,
    GRADIENT_MODULATE_ADD=1,
    GRADIENT_MODULATE_MULTIPLY=2,
  };

private:
  Gtk::Frame frame_pos, frame_light;
  Gtk::Table table_pos, table_light;

  Gtk::Widget *w_rot_x, *w_rot_z, *w_distance;

  REAL_SETTING(x_rotation);
  REAL_SETTING(z_rotation);
  REAL_SETTING(distance);

  INTEGER_SETTING(light_type);
  COLOR_SETTING(light_color);
  REAL_SETTING(influence_night);
  REAL_SETTING(light_intensity);
  REAL_SETTING(light_on_planet);
  REAL_SETTING(light_on_ring);
  REAL_SETTING(specular_factor);
  REAL_SETTING(ring_shadow);
  REAL_SETTING(cloud_shadow);

  sigc::signal<void> _signal_rotation_changed;

public:
  class GradientSetting
  {
    sigc::signal<void> _signal_something_changed;
  public:
    sigc::signal<void>& signal_something_changed(){return _signal_something_changed;}

    Gtk::Expander w_expand;
    Gtk::Table w_table;
    Gtk::HBox expand_title;
    Gtk::Label expand_title_text;
    Gtk::Image expand_title_eye;

    void init_widgets(guint light_id, int id, SettingsWidget& settings);

    Gtk::Widget* w_use;

    BOOLEAN_SETTING(use);
    REAL_SETTING(multiply_gradient_color_with_light_color);
    //REAL_SETTING(multiply_offset_with_calculated_pos);
    //REAL_SETTING(multiply_offset_with_one_minus_calculated_pos);
    REAL_SETTING(radius);
    //REAL_SETTING(offset_x);
    //REAL_SETTING(offset_y);
    REAL_SETTING(inside_planet);
    REAL_SETTING(outside_planet);
    INTEGER_SETTING(modulate_type);
    //INTEGER_SETTING(modulate_gradients);
    //GRADIENT_SETTING(radial_griadient);
    GRADIENT_SETTING(light_gradient);

    GradientSetting();
  };

  GradientSetting gradient0, gradient1, gradient2, gradient3;

  void rotate_z(gfloat a)
  {
    z_rotation += a;
    recalc_pos();
    _signal_z_rotation_changed.emit();
  }
  void rotate_x(gfloat a)
  {
    x_rotation += a;
    recalc_pos();
    _signal_x_rotation_changed.emit();
  }
  void recalc_pos();
  Vector3 position, direction;

  sigc::signal<void>& signal_rotation_changed(){return _signal_rotation_changed;}
  void create_shaders();

  LightLayer(guint id);
  ~LightLayer()throw();
};

void register_light_layer(guint i);

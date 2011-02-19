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

LightLayer::LightLayer(guint id) : MultiLayer<LightLayer>(Glib::ustring::compose(_("LightLayer%1"), id), id==0)
{
  Gtk::Widget* last_set_proprty_widget=nullptr;
  SettingsWidget& settings = *(new SettingsWidget);

  Glib::ustring prefix = Glib::ustring::compose(_("light-%1"), id);

  prepare_settings(prefix, &settings);

#define INIT_TABLE(n, c)\
  frame_##n.add(table_##n);\
  frame_##n.show();\
  table_##n.show();\
  table_##n.set_spacings(LENGTH_SMALLSPACE);\
  table_##n.set_border_width(LENGTH_SMALLSPACE);\
  frame_##n.set_label(_(c));\
  settings.attach_full_line(frame_##n);

#define INIT_GRADIENT(n)\
  gradient##n.init_widgets(id, n, settings);\
  gradient##n.signal_something_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));

#define INIT_PROPERTY(t, c_name, def, name, description)\
  c_name = def;\
  signal_##c_name##_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));\
  last_set_proprty_widget = &settings.append_##t##_widget(TABLE, n, prefix+"-"#c_name, _(name), _(description), X_GETTER_SETTER_SIGNAL(SETTINGS, c_name));

#define INIT_GRADIENT_PROPERTY(t, c_name, def, name, description)\
  c_name = def;\
  signal_##c_name##_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));\
  last_set_proprty_widget = &settings.append_##t##_widget(TABLE, n, prefix+"-"#c_name, _(name), _(description), def);

#define INIT_ENUM_PROPERTY(c_name, def, name, description, e)\
  c_name = def;\
  signal_##c_name##_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));\
  last_set_proprty_widget = &settings.append_enum_widget(TABLE, n, prefix+"-"#c_name, _(name), _(description), e, X_GETTER_SETTER_SIGNAL(SETTINGS, c_name));

#define INIT_REAL_PROPERTY(c_name, def, name, description, step_increment, page_increment, n_digits)\
  c_name = def;\
  signal_##c_name##_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));\
  last_set_proprty_widget = &settings.append_real_widget(TABLE, n, prefix+"-"#c_name, _(name), _(description), X_GETTER_SETTER_SIGNAL(SETTINGS, c_name), step_increment, page_increment, n_digits);

  w_distance = nullptr;
  w_area_diameter = nullptr;

  INIT_TABLE(pos, "Placement")
  INIT_TABLE(light, "Lightning")

  INIT_GRADIENT(0)
  INIT_GRADIENT(1)
  INIT_GRADIENT(2)
  INIT_GRADIENT(3)

  signal_x_rotation_changed().connect(sigc::mem_fun(_signal_rotation_changed, &sigc::signal<void>::emit));
  signal_z_rotation_changed().connect(sigc::mem_fun(_signal_rotation_changed, &sigc::signal<void>::emit));

  guint n=0;
#define TABLE table_pos
#define SETTINGS LightLayer

  INIT_PROPERTY(real, x_rotation, 45.f, "X Angle", "The rotation around the x axis to define the light source's position");
  INIT_PROPERTY(real, z_rotation, 45.f, "Z Angle", "The rotation around the z axis to define the light source's position");
  INIT_PROPERTY(real, distance, 4.f, "Distance", "The distance of the light source to the planetcenter");
  w_distance = last_set_proprty_widget;

#undef TABLE
  n = 0;
#define TABLE table_light

  std::vector<Glib::ustring> enum_light_type = create_vector<Glib::ustring>(_("Ambient"), _("Directional"), _("Point"), _("Area"));
  Gdk::Color light_color_def_value;

  light_color_def_value.set_rgb_p(1., 1., 1.);

  INIT_ENUM_PROPERTY(light_type, LIGHT_TYPE_AREA, "Lighttype", "The Type of the Lightsource", enum_light_type);
  INIT_REAL_PROPERTY(area_diameter, 0.5f, "Area Width", "The diameter of the area light.", 0.1, 0.2, 3);
  w_area_diameter = last_set_proprty_widget;
  INIT_PROPERTY(color, light_color, light_color_def_value, "Color", "The color of the light emitted by this source.");
  INIT_REAL_PROPERTY(light_intensity, 0.5f, "Intensity", "The Intensity of the light emitted by this source.", 0.1, 1., 3);
  INIT_REAL_PROPERTY(light_on_planet, 1.0f, "PlanetIntensity", "The amount of the light which is calculated on the planet's surface.\nThis value doesn't influence the gradient settings.", 0.1, 0.2, 3);
  INIT_REAL_PROPERTY(light_on_ring, 1.0f, "RingIntensity", "The amount of the light which is calculated on the ring's surface.", 0.1, 0.2, 3);
  INIT_REAL_PROPERTY(specular_factor, 1.0f, "Specular", "The amount of the light which is calculated for specular lightning.", 0.1, 0.2, 3);
  INIT_REAL_PROPERTY(ring_shadow, 1.0f, "RingShadow", "The strength of the ring's shadow.", 0.1, 0.2, 3);
  INIT_REAL_PROPERTY(cloud_shadow, 1.0f, "CloudShadow", "The strength of the cloud's shadow.", 0.1, 0.2, 3);

#undef TABLE
#undef SETTINGS
}

LightLayer::~LightLayer()throw()
{
}

void LightLayer::set_x_rotation(gfloat x)
{
  x_rotation = x;
  signal_x_rotation_changed().emit();
}

void LightLayer::set_z_rotation(gfloat x)
{
  z_rotation = x;
  signal_z_rotation_changed().emit();
}

void LightLayer::set_distance(gfloat x)
{
  distance = CLAMP(x, 1.001f, G_MAXFLOAT);
  signal_distance_changed().emit();
}

// ----

void LightLayer::set_light_type(int x)
{
  light_type = CLAMP(x, 0, LIGHT_TYPE_AREA);

  if(w_distance)
    w_distance->set_sensitive(light_type == LIGHT_TYPE_POINT || light_type == LIGHT_TYPE_AREA);
  if(w_area_diameter)
    w_area_diameter->set_sensitive(light_type == LIGHT_TYPE_AREA);

  signal_light_type_changed().emit();
}

void LightLayer::set_area_diameter(gfloat x)
{
  area_diameter = CLAMP(x, 0.f, G_MAXFLOAT);
  signal_area_diameter_changed().emit();
}

void LightLayer::set_light_color(const Gdk::Color& x)
{
  light_color = x;
  signal_light_color_changed().emit();
}

void LightLayer::set_light_intensity(gfloat x)
{
  light_intensity = CLAMP(x, 0.f, G_MAXFLOAT);
  signal_light_intensity_changed().emit();
}

void LightLayer::set_light_on_planet(gfloat x)
{
  light_on_planet = CLAMP(x, 0.f, 10.f);
  signal_light_on_planet_changed().emit();
}

void LightLayer::set_light_on_ring(gfloat x)
{
  light_on_ring = CLAMP(x, 0.f, 10.f);
  signal_light_on_ring_changed().emit();
}

void LightLayer::set_specular_factor(gfloat x)
{
  specular_factor = CLAMP(x, 0.f, 10.f);
  signal_specular_factor_changed().emit();
}

void LightLayer::set_ring_shadow(gfloat x)
{
  ring_shadow = CLAMP(x, 0.f, 10.f);
  signal_ring_shadow_changed().emit();
}

void LightLayer::set_cloud_shadow(gfloat x)
{
  cloud_shadow = CLAMP(x, 0.f, 10.f);
  signal_cloud_shadow_changed().emit();
}

// --------

LightLayer::GradientSetting::GradientSetting()
{

}

void LightLayer::GradientSetting::init_widgets(guint light_id, int id, SettingsWidget& settings)
{
  w_use = nullptr;

  Gtk::Widget* last_set_proprty_widget=nullptr;

  w_expand.show();

  w_expand.add(w_table);
  w_table.show();
  w_table.set_spacings(LENGTH_SMALLSPACE);
  w_table.set_border_width(LENGTH_SMALLSPACE);

  w_expand.set_label_widget(expand_title);
  expand_title.show();
  expand_title.set_spacing(LENGTH_SMALLSPACE);
  expand_title.pack_start(expand_title_eye, false, false);
  expand_title.pack_start(expand_title_text);
    expand_title_text.set_label(Glib::ustring::compose(_("Griadient%1"), id));
    expand_title_text.show();
    expand_title_eye.set(LayerModel::get_singletonA()->pb_invisible);
    expand_title_eye.show();

#define TABLE w_table
#define SETTINGS GradientSetting
  guint n=0;

  Glib::ustring prefix = Glib::ustring::compose(_("light-%1-gradient-%2"), light_id, id);
  std::vector<Glib::ustring> enum_modulate_type = create_vector<Glib::ustring>(_("Normal"), _("Add"), _("Multiply"));

  INIT_PROPERTY(boolean, use, false, "Use", "The distance of the light source to the planetcenter");
  INIT_REAL_PROPERTY(multiply_gradient_color_with_light_color, 1.0f, "MultWithLightColor", "The color of the Gradient will be multiplied with the Light color.", 0.1, 0.2, 3);
  INIT_REAL_PROPERTY(radius, 1.0f, "Radius", "The Radius of the Radialgradient, when the light is directly behind the planet and the inverse, if the light is directly in front of the planet.\nThe distance of the Light is ignored.", 0.1, 0.2, 3);
  INIT_REAL_PROPERTY(inside_planet, 1.0f, "InsidePlanet", "Visibility inside the planet", 0.1, 0.2, 3);
  INIT_REAL_PROPERTY(outside_planet, 0.0f, "OutsidePlanet", "Visibility inside the planet", 0.1, 0.2, 3);
  INIT_ENUM_PROPERTY(modulate_type, GRADIENT_MODULATE_ADD, "ModulateType", "The Type of the Modulation of th gradient", enum_modulate_type);
  INIT_GRADIENT_PROPERTY(gradient, light_gradient, Gradient::create_black2white(), "LightGradient", "The Color Gradient of the Light");

  settings.attach_full_line(w_expand);
}

void LightLayer::GradientSetting::set_use(bool x)
{
  use = x;

  expand_title_eye.set(x ? LayerModel::get_singletonA()->pb_visible : LayerModel::get_singletonA()->pb_invisible);

  signal_use_changed().emit();
}

void LightLayer::GradientSetting::set_multiply_gradient_color_with_light_color(gfloat x)
{
  multiply_gradient_color_with_light_color = CLAMP(x, 0.f, 1.f);
  signal_multiply_gradient_color_with_light_color_changed().emit();
}

void LightLayer::GradientSetting::set_radius(gfloat x)
{
  radius = CLAMP(x, 0.f, G_MAXFLOAT);
  signal_radius_changed().emit();
}

void LightLayer::GradientSetting::set_inside_planet(gfloat x)
{
  inside_planet = CLAMP(x, 0.f, 1.f);
  signal_inside_planet_changed().emit();
}

void LightLayer::GradientSetting::set_outside_planet(gfloat x)
{
  outside_planet = CLAMP(x, 0.f, 1.f);
  signal_outside_planet_changed().emit();
}

void LightLayer::GradientSetting::set_modulate_type(int x)
{
  modulate_type = CLAMP(x, 0, GRADIENT_MODULATE_MULTIPLY);
  signal_modulate_type_changed().emit();
}

void LightLayer::GradientSetting::set_light_gradient(const GradientPtr& x)
{
  *light_gradient.operator->() = *x.operator->();
  signal_light_gradient_changed().emit();
}

void register_light_layer(guint id)
{
  LayerModel::add_layer(Glib::RefPtr<Layer>(new LightLayer(id)));
}

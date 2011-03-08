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

#include "Model.hpp"
#include "SettingsWidget.hpp"

AtmosphereLayer::AtmosphereLayer() : SingletonLayer<AtmosphereLayer>(_("Atmosphere"), true)
{
  SettingsWidget* settings  = new SettingsWidget;
  prepare_settings("atmosphere-texture", settings);

  outer_radius = 1.1;
  inner_gradient = Gradient::create(Gradient::PRESENT_TRANSPARENT_2_WHITE);
  inner_gradient->force_use_alpha(true);

  //inner_gradient_brightness = Curve::create();

  inner_gradient_alpha = Curve::create();
  g_assert(inner_gradient_alpha->get_n_points()==2);
  inner_gradient_alpha->get_point(1).x = 0.15;

  inner_gradient_additive = Gradient::create(Gradient::PRESENT_BLACK_2_WHITE);
  inner_gradient_additive->force_use_alpha(false);

  //inner_gradient_additive_brightness = Curve::create();

  outer_gradient = Gradient::create(Gradient::PRESENT_TRANSPARENT_2_WHITE);
  outer_gradient->flip_h();
  outer_gradient->force_use_alpha(true);

  signal_inner_gradient_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));
  settings->append_gradient_widget("atmosphere-inner-gradient", _("InnerGradient"), _("Describes the color and visibility of the inner Atmosphere depending on the screenspace distance to the planets center."), inner_gradient);

  //signal_inner_gradient_brightness_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));
  //settings->append_curve_widget("atmosphere-inner-gradientsdfsdfsdf", _("InnerGrsdfsdfadfadsfadient"), _("Describes how the atmoshere gets darkafasdfasdf depending an the ."), inner_gradient_brightness);

  signal_inner_gradient_alpha_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));
  settings->append_curve_widget("atmosphere-inner-gradient-diff-alpha", _("InnerGradientDiffAlpha"), _("Describes how the atmoshere gets transparent depending an the diffuse light."), inner_gradient_alpha);

  signal_inner_gradient_additive_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));
  settings->append_gradient_widget("atmosphere-inner-gradient-additive", _("InnerGradientAdditive"), _("Describes the color and visibility of the inner Atmosphere depending on the screenspace distance to the planets center."), inner_gradient_additive);

  signal_outer_gradient_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));
  settings->append_gradient_widget("atmosphere-outer-gradient", _("OuterGradient"), _("Describes the color and visibility of the outer Atmosphere depending on the screenspace distance to the planets center."), outer_gradient);

  signal_outer_radius_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));
  settings->append_real_widget("atmosphere-outer-radius", _("OuterRadius"), _("Describes the thickness of the atmosphere"), X_GETTER_SETTER_SIGNAL(AtmosphereLayer, outer_radius), 0.025, 0.1, 3);
}

AtmosphereLayer::~AtmosphereLayer()throw()
{
}

void AtmosphereLayer::set_outer_radius(float x)
{
  outer_radius = CLAMP(x, 1.f, 2.f);
  signal_outer_radius_changed().emit();
}

void register_atmosphere_layer()
{
  LayerModel::add_layer(Glib::RefPtr<Layer>(new AtmosphereLayer));
}

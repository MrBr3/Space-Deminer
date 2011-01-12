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

RingLayer::RingLayer() : ParentClass(_("Ring"), true)
{
  SettingsWidget* settings  = new SettingsWidget;
  prepare_settings("ring-texture", settings);

  inner_radius  = 1.5f;
  outer_radius  = 2.f;

  x_rotation = 0.f;
  z_rotation = 0.f;

  set_visible(false);

  signal_x_rotation_changed().connect(sigc::mem_fun(_signal_rotation_changed, &sigc::signal<void>::emit));
  signal_z_rotation_changed().connect(sigc::mem_fun(_signal_rotation_changed, &sigc::signal<void>::emit));
  signal_rotation_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));
  signal_inner_radius_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));
  signal_outer_radius_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));

  settings->append_imagefile_widget("ring-texture-file", "File", "The File the Texture is created from", get_imagefile());
  settings->append_real_widget("ring-texture-x-rotation", _("x Rotation"), _("The Rotation along the x axis"), X_GETTER_SETTER_SIGNAL(RingLayer, x_rotation));
  settings->append_real_widget("ring-texture-y-rotation", _("y Rotation"), _("The Rotation along the y axis"), X_GETTER_SETTER_SIGNAL(RingLayer, z_rotation));
  settings->append_real_widget("ring-texture-inner-radius", _("inner Radius"), _("The inner radius of the rings"), X_GETTER_SETTER_SIGNAL(RingLayer, inner_radius));
  settings->append_real_widget("ring-texture-outer-radius", _("outer Radius"), _("The outer radius of the rings"), X_GETTER_SETTER_SIGNAL(RingLayer, outer_radius));

  get_imagefile()->set_filename(apply_filename_macros("$(local-folder)/start-up-planet-images/sup-ring.png")); // TODO you shouldn't need to use full path here
}

RingLayer::~RingLayer()throw()
{
}

void register_ring_layer()
{
  LayerModel::add_layer(Glib::RefPtr<Layer>(new RingLayer));
}
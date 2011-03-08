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

CloudTextureLayer::CloudTextureLayer() : ParentClass(_("Cloud Texture"), true, ImageFile::CONTRAST_CURVE)
{
  SettingsWidget* settings  = new SettingsWidget;
  prepare_settings("cloud-texture", settings);
  settings->append_imagefile_widget("cloud-texture-file", "File", "The File the Texture is created from", get_imagefile());

  cloud_gradient = Gradient::create(Gradient::PRESENT_FULL_WHITE);\
  signal_cloud_gradient_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));\
  settings->append_gradient_widget("night-texture-cloud_gradient", _("CloudGradient"), _("Describes, the Color/Transparency depending on the diffuse lightment"), cloud_gradient);
}

CloudTextureLayer::~CloudTextureLayer()throw()
{
}

void register_cloud_texture_layer()
{
  LayerModel::add_layer(Glib::RefPtr<Layer>(new CloudTextureLayer));
}

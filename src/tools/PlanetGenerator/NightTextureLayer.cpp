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

NightTextureLayer::NightTextureLayer() : ParentClass(_("Night Texture"), true)
{
  SettingsWidget* settings  = new SettingsWidget;
  prepare_settings("night-texture", settings);
  settings->append_imagefile_widget("night-texture-file", "File", "The File the Texture is created from", get_imagefile());

  night_gradient = Gradient::create(Gradient::PRESENT_BLACK_2_WHITE)->force_use_alpha(false);\
  signal_night_gradient_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));\
  settings->append_gradient_widget("night-texture-night_gradient", _("NightGradient"), _("Describes, how the Night Textures gets faded in depending on the lightning."), night_gradient);
}

NightTextureLayer::~NightTextureLayer()throw()
{
}

void register_night_texture_layer()
{
  LayerModel::add_layer(Glib::RefPtr<Layer>(new NightTextureLayer));
}

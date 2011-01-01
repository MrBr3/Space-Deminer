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

 #include "Raytracer.hpp"

namespace Raytracer
{
  Texture* Texture::base_texture = nullptr;
  Texture* Texture::night_texture = nullptr;
  Texture* Texture::weight_map = nullptr;
  Texture* Texture::cloud_layer = nullptr;

  Texture::Texture()
  {
    should_reload = true;
    load_small_version  = false;
  }

  Texture::~Texture()throw()
  {
    if(base_texture==this)
      base_texture = nullptr;
    else if(night_texture==this)
      night_texture = nullptr;
    else if(weight_map==this)
      weight_map = nullptr;
    else if(cloud_layer==this)
      cloud_layer = nullptr;
    else g_assert_not_reached();
  }

  void Texture::reset_any_filename(Glib::ustring fn)
  {
    load_small_version  = false;
    Settings& settings  = Manager::get_settings();

    if(settings.get_use_large_texture())
    {
      str_replace_last_with(fn, '/', settings.get_replace_lt_last_slash_with());
      str_replace_last_with(fn, '.', settings.get_replace_lt_last_dot_with());
      str_replace_all_with(fn, settings.get_replace_lt_every_what(), settings.get_replace_lt_with());
    }

    std::cout<<"loading image <"<<fn<<">\n";

    if(fn!=filename)
    {
      filename  = fn;
      should_reload = true;
    }
  }

  void Texture::reset_base_filename()
  {
    reset_any_filename(BaseTextureLayer::get_imagefile()->get_filename());
  }

  void Texture::reset_night_filename()
  {
    reset_any_filename(NightTextureLayer::get_imagefile()->get_filename());
  }

  void Texture::reset_weight_filename()
  {
    reset_any_filename(WeightTextureLayer::get_imagefile()->get_filename());
  }

  void Texture::reset_cloud_filename()
  {
    reset_any_filename(CloudTextureLayer::get_imagefile()->get_filename());
  }

  void Texture::init()
  {
    if(&Manager::get_singletonA()->base_texture == this)
    {
      base_texture  = this;
      reset_filename  = sigc::mem_fun(*this, &Texture::reset_base_filename);
    }else if(&Manager::get_singletonA()->night_texture == this)
    {
      night_texture  = this;
      reset_filename  = sigc::mem_fun(*this, &Texture::reset_night_filename);
    }else if(&Manager::get_singletonA()->weight_map == this)
    {
      weight_map  = this;
      reset_filename  = sigc::mem_fun(*this, &Texture::reset_weight_filename);
    }else if(&Manager::get_singletonA()->cloud_layer == this)
    {
      cloud_layer  = this;
      reset_filename  = sigc::mem_fun(*this, &Texture::reset_cloud_filename);
    }
  }
}
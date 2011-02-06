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

 #include "./../MainWindow.hpp"

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
    visible = true;

    dummy_color.set(0.f, 0.f, 0.f, .0f);
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

  void Texture::get_color(ColorRGBA& color, gfloat u, gfloat v, WrapMode u_mode, WrapMode v_mode)const
  {
    if(!this || !visible)
    {
      color = dummy_color;
      return;
    }
    if(u_mode==WRAP_TRANSPARENT && !between(u, 0.f, 1.f))
    {
      color.set(0.f, 0.f, 0.f, 0.f);
      return;
    }
    if(v_mode==WRAP_TRANSPARENT && !between(v, 0.f, 1.f))
    {
      color.set(0.f, 0.f, 0.f, 0.f);
      return;
    }
    if(u_mode==WRAP_CLAMPED)
      u = CLAMP(u, 0.f, 1.f);
    if(v_mode==WRAP_CLAMPED)
      v = CLAMP(v, 0.f, 1.f);

    if(!pixbuf || !pixbuf->get_width() || !pixbuf->get_height() || !pixbuf->get_rowstride() || !pixbuf->get_pixels())
    {
      g_assert_not_reached();
      color.set(0.f, 0.f, 0.f, 0.f);
      return;
    }

    int w = pixbuf->get_width();
    int h = pixbuf->get_height();
    guint8* pixels  = pixbuf->get_pixels();
    gsize rowstride = pixbuf->get_rowstride();
    gsize n_channels  = pixbuf->get_n_channels();

    u *= w;
    v *= h;

    gsize x_w  = floor(u);
    gsize x_e  = ceil(u);
    gsize y_n  = floor(v);
    gsize y_s  = ceil(v);

    if(u_mode==WRAP_REPEAT)
    {
      x_w %= w;
      x_e %= w;
    }
    if(v_mode==WRAP_REPEAT)
    {
      y_n %= h;
      y_s %= h;
    }

    g_assert(x_w>=0 && x_w<w);
    g_assert(x_e>=0 && x_e<w);
    g_assert(y_s>=0 && y_s<h);
    g_assert(y_n>=0 && y_n<h);

    ColorRGBA c1, c2, c3, c4;

    c1.set(pixels+rowstride*y_n+n_channels*x_w, n_channels);
    c2.set(pixels+rowstride*y_s+n_channels*x_w, n_channels);
    c3.set(pixels+rowstride*y_s+n_channels*x_e, n_channels);
    c4.set(pixels+rowstride*y_n+n_channels*x_e, n_channels);

    gfloat frac_part_u  = u-x_w;
    gfloat one_minus_frac_part_u  = 1.f-frac_part_u;
    gfloat frac_part_v  = v-y_n;
    gfloat one_minus_frac_part_v  = 1.f-frac_part_v;

    c1.set_mix_of(c1, c2, one_minus_frac_part_v, frac_part_v);
    c2.set_mix_of(c4, c3, one_minus_frac_part_v, frac_part_v);
    color.set_mix_of(c1, c2, one_minus_frac_part_u, frac_part_u);
  }

  void Texture::reload_file()
  {
    if(visible)
    {
      Process::DoingProcessStep _dps("Loading "+texture_name, Process::PROCESS_RENDER);

      Settings& settings  = Manager::get_settings();

      try
      {
        if(load_small_version && !settings.get_use_large_texture())
        {
          pixbuf.reset();
        }else if(load_small_version)
        {
          pixbuf.reset();
          pixbuf  = Gdk::Pixbuf::create_from_file(filename_small);
        }else if(should_reload)
        {
          pixbuf.reset();

          pixbuf  = Gdk::Pixbuf::create_from_file(apply_filename_macros(filename));
        }
      }CATCH_ALL("**Raytracer::Texture::reload_file** ", NOTHING_MACRO)

      //if(!pixbuf)  // TODO dummy texture
    }

    load_small_version  = false;
    should_reload  = false;
  }

  void Texture::reset_any_filename(Glib::ustring fn)
  {
    filename_small  = fn;

    load_small_version  = false;
    Settings& settings  = Manager::get_settings();

    if(settings.get_use_large_texture())
    {
      str_replace_last_with(fn, '/', settings.get_replace_lt_last_slash_with());
      str_replace_last_with(fn, '.', settings.get_replace_lt_last_dot_with());
      str_replace_all_with(fn, settings.get_replace_lt_every_what(), settings.get_replace_lt_with());
    }

    if(fn!=filename)
    {
      filename  = fn;
      should_reload = true;
    }

    if(!pixbuf)
      should_reload  =true;
  }

  void Texture::reset_base_filename()
  {
    visible = BaseTextureLayer::get_singleton()->get_visible();
    reset_any_filename(BaseTextureLayer::get_imagefile()->get_filename());
  }

  void Texture::reset_night_filename()
  {
    visible = NightTextureLayer::get_singleton()->get_visible();
    reset_any_filename(NightTextureLayer::get_imagefile()->get_filename());
  }

  void Texture::reset_weight_filename()
  {
    visible = WeightTextureLayer::get_singleton()->get_visible();
    reset_any_filename(WeightTextureLayer::get_imagefile()->get_filename());
  }

  void Texture::reset_cloud_filename()
  {
    visible = CloudTextureLayer::get_singleton()->get_visible();
    reset_any_filename(CloudTextureLayer::get_imagefile()->get_filename());
  }

  void Texture::init()
  {
    if(&Manager::get_singletonA()->base_texture == this)
    {
      base_texture  = this;
      reset_filename  = sigc::mem_fun(*this, &Texture::reset_base_filename);
      texture_name  = _("Base Texture");
    }else if(&Manager::get_singletonA()->night_texture == this)
    {
      night_texture  = this;
      reset_filename  = sigc::mem_fun(*this, &Texture::reset_night_filename);
      texture_name  = _("Night Texture");
    }else if(&Manager::get_singletonA()->weight_map == this)
    {
      weight_map  = this;
      reset_filename  = sigc::mem_fun(*this, &Texture::reset_weight_filename);
      texture_name  = _("Weight Map");
    }else if(&Manager::get_singletonA()->cloud_layer == this)
    {
      cloud_layer  = this;
      reset_filename  = sigc::mem_fun(*this, &Texture::reset_cloud_filename);
      texture_name  = _("Cloud Layer");
    }
  }
}
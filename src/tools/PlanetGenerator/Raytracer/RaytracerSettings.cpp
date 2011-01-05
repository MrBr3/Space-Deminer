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
  Settings::Settings()
  {
    guint n=0;

    attach_full_line(frame_dest_file);
    attach_full_line(frame_performance);
    attach_full_line(frame_debugging);
    attach_full_line(frame_textures);

    frame_dest_file.add(table_dest_file);
    frame_performance.add(table_performance);
    frame_debugging.add(table_debugging);
    frame_textures.add(table_textures);

    table_dest_file.set_spacings(LENGTH_SMALLSPACE);
    table_performance.set_spacings(LENGTH_SMALLSPACE);
    table_debugging.set_spacings(LENGTH_SMALLSPACE);
    table_textures.set_spacings(LENGTH_SMALLSPACE);
    table_dest_file.set_border_width(LENGTH_SMALLSPACE);
    table_performance.set_border_width(LENGTH_SMALLSPACE);
    table_debugging.set_border_width(LENGTH_SMALLSPACE);
    table_textures.set_border_width(LENGTH_SMALLSPACE);

    frame_dest_file.set_label(_("Dest. Image"));
    frame_performance.set_label(_("Performance"));
    frame_debugging.set_label(_("Multi Pass"));

    width = 2048;
    signal_width_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));
    append_int_widget(table_dest_file, n, "raytrace-width", _("Width"), _("The width of the resulting image"), X_GETTER_SETTER_SIGNAL(Settings, width));

    height = 2048;
    signal_height_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));
    append_int_widget(table_dest_file, n, "raytrace-height", _("Height"), _("The height of the resulting image"), X_GETTER_SETTER_SIGNAL(Settings, height));

    dithering = true;
    signal_dithering_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));
    append_boolean_widget(table_dest_file, n, "raytrace-dithering", _("Use Dithering"), _("If set, the resulting image will be dithered"), X_GETTER_SETTER_SIGNAL(Settings, dithering));

    dest_file = "~/Desktop/planet.png";
    signal_dest_file_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));
    append_string_widget(table_dest_file, n, "raytrace-dest-file", _("Dest. File"), _("The filename, the image will be saved, to\n\n")+get_filename_macros_description(), X_GETTER_SETTER_SIGNAL(Settings, dest_file));

    overwrite = true;
    signal_overwrite_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));
    append_boolean_widget(table_dest_file, n, "raytrace-overwrite", _("Overwrite"), _("If set, a possibly existing file will be overwritten withour asking"), X_GETTER_SETTER_SIGNAL(Settings, overwrite));

    //================
    n=0;

    n_render_tiles  = 8;
    signal_n_render_tiles_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));
    append_int_widget(table_performance, n, "raytrace-render-tiles", _("RenderTiles"), _("The number of render-tiles along one axis used in order to use mutltithreading to fasten redering"), X_GETTER_SETTER_SIGNAL(Settings, n_render_tiles));

    antialiasing  = 3;
    signal_antialiasing_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));
    append_enum_widget(table_performance, n, "raytrace-antialiasing", _("Antialiasing"), _("The amount of Antialiasing"), create_vector<Glib::ustring>("1 ray per Pixel", "2 rays per Pixel", "4 rays per Pixel", "8 rays per Pixel"), X_GETTER_SETTER_SIGNAL(Settings, antialiasing));


    //================
    n=0;

    save_also_normal  = true;
    signal_save_also_normal_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));
    append_boolean_widget(table_debugging, n, "raytrace-save-also-normal", _("Normals"), _("If set, the normals will be also saved"), X_GETTER_SETTER_SIGNAL(Settings, save_also_normal));

    save_also_uv  = true;
    signal_save_also_uv_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));
    append_boolean_widget(table_debugging, n, "raytrace-save-also-uv", _("UV"), _("If set, the UV-Coordinates wil be also saved"), X_GETTER_SETTER_SIGNAL(Settings, save_also_uv));

    save_also_unlit_base_texture  = true;
    signal_save_also_unlit_base_texture_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));
    append_boolean_widget(table_debugging, n, "raytrace-save-also-unlit-base-texture", _("BaseTexture"), _("If set, a unlit version of the base texture wil be also saved"), X_GETTER_SETTER_SIGNAL(Settings, save_also_unlit_base_texture));

    //================
    n=0;

    {use_large_texture = false;
    signal_use_large_texture_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));
    Gtk::Widget& label = append_boolean_widget(table_textures, n, "raytrace-use-large-texture", _("Use large Textures"), _("If set, PlanetGenerator will search for larger Textures using the hints below"), X_GETTER_SETTER_SIGNAL(Settings, use_large_texture));
    table_textures.remove(label);
    frame_textures.set_label_widget(label);}

    table_textures.set_sensitive(use_large_texture);
    signal_use_large_texture_changed().connect(create_updater<bool, bool>(sigc::mem_fun(*this, &Settings::get_use_large_texture), sigc::mem_fun(table_textures, &Gtk::Widget::set_sensitive)));

    n=0;

    replace_lt_last_slash_with = "/";
    signal_replace_lt_last_slash_with_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));
    append_string_widget(table_textures, n, "raytrace-replace-last-slash-with", _("ReplLastSlashWith"), _("The last slash of the Texture source filename will be replaced by this value"), X_GETTER_SETTER_SIGNAL(Settings, replace_lt_last_slash_with));

    replace_lt_last_dot_with = "-large.";
    signal_replace_lt_last_dot_with_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));
    append_string_widget(table_textures, n, "raytrace-replace-last-dot-with", _("ReplLastDotWith"), _("The last dot of the Texture source filename will be replaced by this value"), X_GETTER_SETTER_SIGNAL(Settings, replace_lt_last_dot_with));

    replace_lt_every_what = "";
    signal_replace_lt_every_what_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));
    append_string_widget(table_textures, n, "raytrace-replace-every-what", _("Replace Every"), _("Every appeareance of this string within the Texture source filename will be replaced by the string below"), X_GETTER_SETTER_SIGNAL(Settings, replace_lt_every_what));

    replace_lt_with = "";
    signal_replace_lt_with_changed().connect(sigc::mem_fun(signal_something_changed(), &sigc::signal<void>::emit));
    append_string_widget(table_textures, n, "raytrace-replace-with", _("Replace With"), _("Every appeareance of the string above within the Texture source filename will be replaced by this string"), X_GETTER_SETTER_SIGNAL(Settings, replace_lt_with));

    show_all_children();
  }

  Settings::~Settings()throw()
  {
  }

  void Settings::set_width(int w)
  {
    width = correct_image_size(w);

    signal_width_changed().emit();
  }

  void Settings::set_height(int h)
  {
    height = correct_image_size(h);

    signal_height_changed().emit();
  }

  void Settings::set_dithering(bool d)
  {
    dithering = d;

    signal_dithering_changed().emit();
  }

  void Settings::set_dest_file(const Glib::ustring& df)
  {
    dest_file = df;

    signal_dest_file_changed().emit();
  }

  void Settings::set_overwrite(bool o)
  {
    overwrite = true; // TODO this value shouldn't be ignored

    signal_overwrite_changed().emit();
  }

  //============

  void Settings::set_n_render_tiles(int n)
  {
    n_render_tiles = CLAMP(n, 1, 32);

    signal_n_render_tiles_changed().emit();
  }

  void Settings::set_antialiasing(int a)
  {
    antialiasing = a;

    signal_antialiasing_changed().emit();
  }

  //============

  void Settings::set_save_also_normal(bool o)
  {
    save_also_normal = true; // TODO this value shouldn't be ignored

    signal_save_also_normal_changed().emit();
  }


  void Settings::set_save_also_uv(bool o)
  {
    save_also_uv= true; // TODO this value shouldn't be ignored

    signal_save_also_uv_changed().emit();
  }


  void Settings::set_save_also_unlit_base_texture(bool o)
  {
    save_also_unlit_base_texture= true; // TODO this value shouldn't be ignored

    signal_save_also_unlit_base_texture_changed().emit();
  }

  //============

  void Settings::set_replace_lt_last_slash_with(const Glib::ustring& ls)
  {
    if(ls.empty())
      replace_lt_last_slash_with  = "/";
    else
      replace_lt_last_slash_with = ls;

    signal_replace_lt_last_slash_with_changed().emit();
  }

  void Settings::set_replace_lt_last_dot_with(const Glib::ustring& ls)
  {
    if(ls.empty())
      replace_lt_last_slash_with  = ".";
    else
      replace_lt_last_slash_with = ls;

    signal_replace_lt_last_dot_with_changed().emit();
  }

  void Settings::set_replace_lt_every_what(const Glib::ustring& w)
  {
    replace_lt_every_what = w;

    signal_replace_lt_every_what_changed().emit();
  }

  void Settings::set_replace_lt_with(const Glib::ustring& w)
  {
    replace_lt_with = w;

    signal_replace_lt_with_changed().emit();
  }

  //============

  void Settings::set_use_large_texture(bool lt)
  {
    use_large_texture = lt; // TODO this value shouldn't be ignored

    signal_use_large_texture_changed().emit();
  }
}
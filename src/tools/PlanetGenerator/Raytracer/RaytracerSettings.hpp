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

#include "./../SettingsWidget.hpp"

namespace Raytracer
{
  const int max_image_size  = 4096;

  class Settings : public SettingsWidget
  {
  public:
    typedef SettingsWidget ParentClass;

  private:
    INTEGER_SETTING(width)
    INTEGER_SETTING(height)
    BOOLEAN_SETTING(dithering)
    STRING_SETTING(dest_file)
    BOOLEAN_SETTING(overwrite)

    INTEGER_SETTING(n_render_tiles)
    INTEGER_SETTING(antialiasing)
    BOOLEAN_SETTING(culling)
    INTEGER_SETTING(culling_epsilon)
    BOOLEAN_SETTING(clear_before_rendering)

    BOOLEAN_SETTING(dbg_normal)
    BOOLEAN_SETTING(dbg_uv)
    BOOLEAN_SETTING(dbg_unlit_base_texture)
    BOOLEAN_SETTING(dbg_culling)
    /** \brief Deactivates all debugging Flags
     *
     * \param deactivate if false nothing will be done
     * */
    void no_debugging(bool deactivate);

    BOOLEAN_SETTING(use_large_texture)
    STRING_SETTING(replace_lt_last_slash_with)
    STRING_SETTING(replace_lt_last_dot_with)
    STRING_SETTING(replace_lt_every_what)
    STRING_SETTING(replace_lt_with)

    Gtk::Frame frame_dest_file;
    Gtk::Table table_dest_file;

    Gtk::Frame frame_performance;
    Gtk::Table table_performance;

    Gtk::Frame frame_debugging;
    Gtk::Table table_debugging;

    Gtk::Frame frame_textures;
    Gtk::Table table_textures;

    sigc::signal<void> _signal_something_changed;

  public:
    sigc::signal<void>& signal_something_changed(){return _signal_something_changed;}

    static guint correct_image_size(guint s)
    {
      return CLAMP(s, 32, max_image_size);
    }

    Glib::Mutex render_mutex;
    guint rendering_threads;

  public:
    Settings();
    ~Settings()throw();
  };
}
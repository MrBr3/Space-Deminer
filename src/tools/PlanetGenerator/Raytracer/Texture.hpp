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

namespace Raytracer
{
  class Texture : public sigc::trackable
  {
  private:
    friend class Manager;
    Texture();
    ~Texture()throw();

  private:
    Glib::RefPtr<Gdk::Pixbuf> pixbuf;
    Glib::ustring filename;

  public:
    Glib::TimeVal last_modification_time;
    bool should_reload;
    bool load_small_version; // if load_small_version==true should_reload will be ignored

  public:
      const Glib::ustring& get_filename()const{return filename;}

  private:
    void init();
    sigc::slot<void> reset_filename;

    void reset_any_filename(Glib::ustring fn);
    void reset_base_filename();
    void reset_night_filename();
    void reset_weight_filename();
    void reset_cloud_filename();

  public:
    static Texture* base_texture;
    static Texture* night_texture;
    static Texture* weight_map;
    static Texture* cloud_layer;
  };
}
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
  class ResultingImage : public Refable
  {
  private:
    guint _width, _height;

    guint max_tile_width;
    guint max_tile_height;

    Glib::RefPtr<Gdk::Pixbuf> _pixbuf;

    sigc::signal<void> _signal_new_pixbuf_created;

  public:
    const Glib::RefPtr<Gdk::Pixbuf>& get_pixbuf(){return _pixbuf;}
    sigc::signal<void>& signal_new_pixbuf_created(){return _signal_new_pixbuf_created;}

    guint get_width()const{return _width;}
    guint get_height()const{return _height;}

    void prepare(bool preview);

    class Tile
    {
    public:
      const guint x, y, w, h;
      bool operator<(const Tile& t)const;

      Tile(guint _x, guint _y, guint _w, guint _h) : x(_x), y(_y), w(_w), h(_h)
      {
      }
    };

    std::list<Tile> tiles;

  private:
    guint _rendering_threads;
    Glib::Mutex _render_mutex;
    Glib::Mutex _invalidate_mutex;
    Glib::Mutex _waiting_for_start_mutex;
    bool _aborted;
    bool _invalidate;

    sigc::signal<void> _signal_invalidated;

    static void render_thread(ResultingImage* ri);
  public:

    void render();

    sigc::signal<void>& signal_invalidated(){return _signal_invalidated;}

  public:
    ResultingImage();
    ~ResultingImage()throw();
  };
}

guint get_n_cores();
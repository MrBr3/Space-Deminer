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

    Glib::RefPtr<Gdk::Pixbuf> _pixbuf;

    sigc::signal<void> _signal_new_pixbuf_created;

  public:
    const Glib::RefPtr<Gdk::Pixbuf>& get_pixbuf(){return _pixbuf;}
    sigc::signal<void>& signal_new_pixbuf_created(){return _signal_new_pixbuf_created;}

    guint get_width()const{return _width;}
    guint get_height()const{return _height;}

    void prepare(bool preview);

    ResultingImage();
    ~ResultingImage()throw();
  };
}
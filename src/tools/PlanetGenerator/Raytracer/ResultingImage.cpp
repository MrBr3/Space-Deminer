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
  bool ResultingImage::Tile::operator<(const Tile& t)const
  {
    g_assert(Manager::get_resulting_image().get_pixbuf());

    guint half_width = Manager::get_resulting_image().get_pixbuf()->get_height()>>1;
    guint half_height = Manager::get_resulting_image().get_pixbuf()->get_height()>>1;

    guint this_d  = abs(x-half_width)*abs(x-half_width) + abs(y-half_height)*abs(y-half_height);
    guint that_d  = abs(t.x-half_width)*abs(t.x-half_width) + abs(t.y-half_height)*abs(t.y-half_height);

    if(this_d!=that_d)
      return this_d<that_d;

    if(x==t.x)
      return x<t.x;

    return y<t.y;
  }

  ResultingImage::ResultingImage()
  {
    _width  = 64;
    _height  = 64;
  }

  ResultingImage::~ResultingImage()throw()
  {
  }

  void ResultingImage::prepare(bool preview)
  {
    Process::PushProcess pp("Preparing pixelbuffer", true, Process::PROCESS_RENDER, 0, true);

    if(preview)
    {
      main_window->get_render_view_size(_width, _height);
    }else
    {
      _width = Manager::get_settings().get_width();
      _height = Manager::get_settings().get_height();
    }

    _width  = Settings::correct_image_size(_width);
    _height  = Settings::correct_image_size(_height);

    if(!_pixbuf || _pixbuf->get_width()!=get_width() || _pixbuf->get_height()!=get_height())
    {
      _pixbuf.reset();
      _pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, get_width(), get_height());
      _pixbuf->fill(0x000000ff);

      signal_new_pixbuf_created().emit();
    }

    tiles.clear();

    const gint n_tiles_axis = Manager::get_settings().get_n_render_tiles();

    guint curr_x = 0;
    guint curr_y = 0;
    for(gint w = _width, nx=n_tiles_axis; w>0; --nx)
    {
      guint curr_width  = ceil(w/gfloat(nx));

      for(gint h = _height, ny=n_tiles_axis; h>0; --ny)
      {
        g_assert(nx>=0);
        g_assert(ny>=0);

        guint curr_height  = ceil(h/gfloat(ny));

        tiles.push_back(Tile(curr_x, curr_y, curr_width, curr_height));

        std::cout<<curr_x<<"  "<<curr_y<<"  "<<curr_width<<"  "<<curr_height<<"\n";

        curr_y  += curr_width;
        h -= curr_height;
      }
      curr_x  += curr_width;
      w -= curr_width;
    }

    g_assert(tiles.size()==n_tiles_axis*n_tiles_axis);

    tiles.sort();
  }
}
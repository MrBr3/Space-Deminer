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
  void calc_pixel_color(ColorRGBA& resulting_color, gfloat x, gfloat y);

  bool ResultingImage::Tile::operator<(const Tile& t)const
  {
    g_assert(Manager::get_resulting_image().get_pixbuf());

    gint image_center_x = Manager::get_resulting_image().get_pixbuf()->get_width()>>1;
    gint image_center_y = Manager::get_resulting_image().get_pixbuf()->get_height()>>1;
    gint half_tile_width = w>>1;
    gint half_tile_height = h>>1;

    gint this_center_x =   x+half_tile_width;
    gint this_center_y =   y+half_tile_height;
    gint that_center_x = t.x+half_tile_width;
    gint that_center_y = t.y+half_tile_height;

    guint this_d  = abs(this_center_x-image_center_x)*abs(this_center_x-image_center_x) + abs(this_center_y-image_center_y)*abs(this_center_y-image_center_y);
    guint that_d  = abs(that_center_x-image_center_x)*abs(that_center_x-image_center_x) + abs(that_center_y-image_center_y)*abs(that_center_y-image_center_y);

    if(this_d!=that_d)
      return this_d<that_d;

    if(x==t.x)
      return x<t.x;

    return y<t.y;
  }

  ResultingImage::ResultingImage() : _waiting_for_start(get_n_cores()+1), _waiting_for_finish(get_n_cores()+1)
  {
    _width  = 64;
    _height  = 64;

    max_tile_width  = 0;
    max_tile_height  = 0;

    _rendering_threads = 0;
    _rendered_tiles = 0;
    _invalidate = false;
    _ui_timer_ready = false;
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
      _pixbuf->fill(0x00000000);

      signal_new_pixbuf_created().emit();
    }else if(Manager::get_settings().get_clear_before_rendering())
    {
      _pixbuf->fill(0x00000000);
    }

    tiles.clear();

    const gint n_tiles_axis = Manager::get_settings().get_n_render_tiles();

    max_tile_width  = 0;
    max_tile_height  = 0;

    guint curr_x = 0;
    for(gint w = _width, nx=n_tiles_axis; w>0; --nx)
    {
      guint curr_width  = ceil(w/gfloat(nx));
      guint curr_y = 0;

      for(gint h = _height, ny=n_tiles_axis; h>0; --ny)
      {
        g_assert(nx>=0);
        g_assert(ny>=0);

        guint curr_height  = ceil(h/gfloat(ny));

        tiles.push_back(Tile(curr_x, curr_y, curr_width, curr_height));

        //std::cout<<curr_x<<"  "<<curr_y<<"  "<<curr_width<<"  "<<curr_height<<"\n";

        max_tile_width  = MAX(curr_width, max_tile_width);
        max_tile_height  = MAX(curr_height, max_tile_height);

        g_assert(curr_x+curr_width<=_width);
        g_assert(curr_y+curr_height<=_height);

        curr_y  += curr_height;
        h -= curr_height;
      }
      curr_x  += curr_width;
      w -= curr_width;
    }

    g_assert(tiles.size()==n_tiles_axis*n_tiles_axis);

    tiles.sort();
  }

  void ResultingImage::render_thread(ResultingImage* ri)
  {
    g_assert(ri);
    g_assert(ri->get_pixbuf());
    g_assert(ri->get_pixbuf()->get_n_channels()==4);

    ri->_render_mutex.lock();

      ri->_rendering_threads++;

      const gsize rowstride = ri->get_pixbuf()->get_rowstride();
      guint8* const all_pixels  = ri->get_pixbuf()->get_pixels();
      bool first_tile = true;

    ri->_render_mutex.unlock();

    gfloat inv_width  = 1.f/gfloat(ri->_width);
    gfloat inv_height = 1.f/gfloat(ri->_height);
    ColorRGBA col(DONT_INIT);

    struct DitherHelper
    {
      ColorRGBA* line;
      ColorRGBA se;

      DitherHelper() : line(nullptr)
      {
      }

      ~DitherHelper()
      {
        delete[] line;
      }
    }dh;

    if(Manager::get_settings().get_dithering())
      dh.line = new ColorRGBA[ri->max_tile_width+2];

    ri->_waiting_for_start.wait();

    while(!ri->_aborted)
    {
      ri->_render_mutex.lock();
        if(ri->tiles.size()==0)
        {
          ri->_render_mutex.unlock();
          break;
        }

        Tile tile = *ri->tiles.begin();
        ri->tiles.pop_front();

        guint8* px  = all_pixels + tile.x*4 + rowstride*tile.y;

       if(first_tile)
       {
         first_tile = false;
       }else
       {
         ri->_rendered_tiles++;
       }

      ri->_render_mutex.unlock();

      for(guint y=0; y<tile.h && !ri->_aborted; ++y)
      {
        px  = all_pixels + tile.x*4 + rowstride*(tile.y+y);

        for(guint x=0; x<tile.w; ++x, px+=4)
        {
          calc_pixel_color(col, gfloat(x+tile.x)*inv_width, gfloat(y+tile.y)*inv_height);

          if(dh.line)
          {
            ColorRGBA temp = dh.se;

            dh.se.set();

            col.fill_dithered(px,
                              dh.line[1+x], // current pixel
                              dh.line[2+x], // eastern pixel
                              dh.se,           // southeaster pixel
                              dh.line[1+x], // southern pixel
                              dh.line[x]);  // southwest pixel

            dh.line[1+x].r  += temp.r;
            dh.line[1+x].g  += temp.g;
            dh.line[1+x].b  += temp.b;
            dh.line[1+x].a  += temp.a;
          }else
            col.fill(px);

          Glib::usleep(50);
        }
      }
    }

    ri->_render_mutex.lock();
    ri->_rendering_threads--;
    ri->_render_mutex.unlock();
    ri->_waiting_for_finish.wait();
  }

  void ResultingImage::render()
  {
    _render_mutex.lock();

    _rendering_threads = 0;
    _rendered_tiles = 0;
    _ui_timer_ready = false;
    _aborted = false;

    _render_mutex.unlock();

    for(guint i=0; i<get_n_cores(); ++i)
      Glib::Thread::create(sigc::bind(sigc::ptr_fun(ResultingImage::render_thread), this), false);

    _waiting_for_start.wait();

    think_ui_timer();

    while(!_aborted && _rendering_threads!=0)
    {
      Gtk::Main::iteration(false);
    }

    _waiting_for_finish.wait();

    g_assert(_aborted || _rendering_threads==0);

    while(!_ui_timer_ready)
    {
      Gtk::Main::iteration(false);
    }

    on_invalidated();

  }

  void ResultingImage::think_ui_timer()
  {
    Glib::signal_timeout().connect_once(sigc::mem_fun(*this, &ResultingImage::ui_timer_on_think), 250);
  }

  void ResultingImage::ui_timer_on_think()
  {
    if(_rendering_threads==0)
    {
      _ui_timer_ready = true;
      return;
    }

    _render_mutex.lock();
      while(_rendered_tiles>0)
      {
        _rendered_tiles--;
        Process::processing_step_done(Process::PROCESS_RENDER);
      }
    _render_mutex.unlock();

    _aborted |= Process::is_curr_process_aborted();

    on_invalidated();

    if(_aborted)
    {
      _ui_timer_ready = true;
    }else
      think_ui_timer();
  }
}
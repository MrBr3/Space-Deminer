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

#include "menu-alignment.hpp"
#include <iostream>

/**********x***********************x***********x*****
 * nw_c    x   e_nr                x e_ne      x    *
 *         x                       x           x    *
 *xxxx+-----+---------------------------+------+    *
 *e_wn|     |                           |      |ne_c*
 *    |t_w  |                           |t_e   |    *
 *    |-----+                           +------+    *
 xxxxx|                                        |xxxxx
 *    |                                        |e_er*
 *    |                                        |xxxxx
 *    |                                        |    *
 *    |                                        |    *
 *    +----------------------------------------+    *
 *                                    xe_se    xse_c*
 *                                    x        x    *
 *************************************x********x****/

namespace ui_frame_metrics
{
  const Real full_w  = 384.;
  const Real full_h  = 384.;
  const Real f_n_h = 32.;
  const Real f_s_h = 32.;
  const Real f_w_w = 32.;
  const Real f_e_w = 32.;

  const Real nw_c_x  = 0.;
  const Real nw_c_y  = 0.;
  const Real nw_c_w  = 183.;
  const Real nw_c_h  = f_n_h;

  const Real ne_c_x  = full_w-f_e_w;
  const Real ne_c_y  = 0.;
  const Real ne_c_w  = f_e_w;
  const Real ne_c_h  = 139.;

  const Real se_c_h  = 228.;
  const Real se_c_w  = f_e_w;
  const Real se_c_x  = ne_c_x;
  const Real se_c_y  = full_h-se_c_h;

  const Real e_er_x  = ne_c_x;
  const Real e_er_y  = ne_c_y+ne_c_h;
  const Real e_er_w  = f_e_w;
  const Real e_er_h  = full_h-ne_c_h-se_c_h;

  const Real e_wn_x  = 0.;
  const Real e_wn_y  = f_n_h;
  const Real e_wn_w  = f_w_w;
  const Real e_wn_h  = 64.;

  const Real e_se_w = 128.;
  const Real e_se_h = f_s_h;
  const Real e_se_x = se_c_x-e_se_w;
  const Real e_se_y = full_h-f_s_h;

  const Real e_ne_w = 154.;
  const Real e_ne_h = f_n_h;
  const Real e_ne_x = ne_c_x-e_ne_w;
  const Real e_ne_y = 0.;

  const Real e_nr_x  = nw_c_x+nw_c_w;
  const Real e_nr_y  = nw_c_y;
  const Real e_nr_w  = full_w-e_nr_x-e_ne_w-ne_c_w;
  const Real e_nr_h  = f_n_h;

  Real t_w_x  = f_w_w;
  Real t_w_y  = f_n_h;
  Real t_w_w  = 123.;
  Real t_w_h  = 64.;

  Real t_e_w  = 22.;
  Real t_e_h  = 64.;
  Real t_e_x  = full_w-f_e_w-t_e_w;
  Real t_e_y  = f_n_h;
}

using namespace ui_frame_metrics;

Real MenuFrame::get_back_color_r(){return MenuAlignment::get_singleton()->menu_frame._back_color;}
Real MenuFrame::get_back_color_g(){return MenuAlignment::get_singleton()->menu_frame._back_color;}
Real MenuFrame::get_back_color_b(){return MenuAlignment::get_singleton()->menu_frame._back_color;}

MenuFrame::MenuFrame()  // TODO put this into an init() function?
{
  Framework::Engine* engine  = Framework::Engine::get_singleton();

  Glib::RefPtr<Gdk::Pixbuf> full_image  = Gdk::Pixbuf::create_from_file(Glib::filename_from_utf8(apply_filename_macros("$(exe-share)/ui/frames/frame-001.png")));

  _icon_back  = engine->create_image(Gdk::Pixbuf::create_subpixbuf(full_image, t_w_x, t_w_y, t_w_w, t_w_h));
  _e_title_bar  = engine->create_image(Gdk::Pixbuf::create_subpixbuf(full_image, t_e_x, t_e_y, t_e_w, t_e_h));

  _nw_corner  = engine->create_image(Gdk::Pixbuf::create_subpixbuf(full_image, nw_c_x, nw_c_y, nw_c_w, nw_c_h));
  _se_corner  = engine->create_image(Gdk::Pixbuf::create_subpixbuf(full_image, se_c_x, se_c_y, se_c_w, se_c_h));
  _ne_corner  = engine->create_image(Gdk::Pixbuf::create_subpixbuf(full_image, ne_c_x, ne_c_y, ne_c_w, ne_c_h));

  _e_edge_bottom_repeat_pattern  = engine->create_image(Gdk::Pixbuf::create_subpixbuf(full_image, e_er_x, e_er_y, e_er_w, e_er_h), TEXTURE_HINT_TILABLE);
  _e_edge_top_repeat_pattern  = engine->create_image(Gdk::Pixbuf::create_subpixbuf(full_image, e_nr_x, e_nr_y, e_nr_w, e_nr_h), TEXTURE_HINT_TILABLE);

  _w_edge_top   = engine->create_image(Gdk::Pixbuf::create_subpixbuf(full_image, e_wn_x, e_wn_y, e_wn_w, e_wn_h));
  _s_edge_right = engine->create_image(Gdk::Pixbuf::create_subpixbuf(full_image, e_se_x, e_se_y, e_se_w, e_se_h));
  _n_edge_top   = engine->create_image(Gdk::Pixbuf::create_subpixbuf(full_image, e_ne_x, e_ne_y, e_ne_w, e_ne_h));

  _back_color =
      full_image->get_pixels()[
         full_image->get_rowstride()*(full_image->get_height()>>1) +
        (full_image->get_bits_per_sample()>>3)*full_image->get_n_channels()*(full_image->get_width()>>1)]
    / 255.f;
}

MenuFrame::~MenuFrame()throw()
{
}

void MenuFrame::expose_background(Framework::Widget::EventExpose& event, const Framework::Widget::Allocation& allocation)
{
  event.draw_color_rect(allocation.get_x()+t_w_w, allocation.get_y(), allocation.get_width()-t_w_w-t_e_w, t_e_h, _back_color, _back_color, _back_color, 1.f);
  event.draw_color_rect(allocation.get_x(), allocation.get_y()+t_e_h, allocation.get_width(), allocation.get_height()-t_e_h, _back_color, _back_color, _back_color, 1.f);

  event.draw_image(_icon_back, allocation.get_x(), allocation.get_y());
  event.draw_image(_e_title_bar, allocation.get_x()+allocation.get_width()-t_e_w, allocation.get_y());
}

void MenuFrame::expose(Framework::Widget::EventExpose& event, const Framework::Widget::Allocation& allocation)
{
  Real a_x       = allocation.get_x();
  Real a_y       = allocation.get_y();
  Real a_width   = allocation.get_width();
  Real a_height  = allocation.get_height();
  Real left   = a_x;
  Real top    = a_y;
  Real right  = a_x + a_width;
  Real bottom = a_y + a_height;

  event.draw_image(_nw_corner, left-32., top-32.);
  event.draw_image(_se_corner, right, bottom+32.-se_c_h);
  event.draw_image(_ne_corner, right, top-32.);

  event.draw_image_tiled(_e_edge_bottom_repeat_pattern, right, top+e_er_y-32., 16., a_height-303.);
  event.draw_image_tiled(_e_edge_top_repeat_pattern, left+e_nr_x-32., top-32., right-left+32.-nw_c_w-e_ne_w, e_nr_h);

  event.draw_image(_w_edge_top, left-32., top);
  event.draw_image(_s_edge_right, right-e_se_w, bottom);

  event.draw_image(_n_edge_top, right-e_ne_w, top-32.);
}
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

#ifndef _SPACE_DEMINER_MENU_FRAME_H_
#define _SPACE_DEMINER_MENU_FRAME_H_

#include "./framework.hpp"

class MenuFrame : public sigc::trackable
{
public:
  MenuFrame();
  ~MenuFrame()throw();

  void expose(Framework::Widget::EventExpose& cc, const Framework::Widget::Allocation& allocation);
  void expose_background(Framework::Widget::EventExpose& cc, const Framework::Widget::Allocation& allocation);

  static Real get_back_color_r();
  static Real get_back_color_g();
  static Real get_back_color_b();
private:
  Framework::ResPtr<Framework::Image> _se_corner, _ne_corner, _nw_corner, _icon_back, _e_title_bar;
  Framework::ResPtr<Framework::Image> _w_edge_top, _s_edge_right, _n_edge_top;
  Framework::ResPtr<Framework::Image> _e_edge_bottom_repeat_pattern, _e_edge_top_repeat_pattern;
  Real _back_color;
};

#endif
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

#ifndef _SPACE_DEMINER_MENU_ALIGNMENT_H_
#define _SPACE_DEMINER_MENU_ALIGNMENT_H_

#include "./menu.hpp"

class MenuAlignment : public StaticManager<MenuAlignment, Framework::Alignment>
{
public:
  MenuBack menu_back;
  MenuFrame menu_frame;

  Framework::VBox split_areas;
    Framework::HBox title_area;
      Framework::ImageWidget page_icon;
      Framework::Alignment link_area;
    Framework::ScrolledWindow main_area_sw;
      Framework::Viewport main_area;
public:
  void on_expose(EventExpose& event);

  void on_size_allocate();

public:
  MenuAlignment();
  ~MenuAlignment()throw();
};

#endif

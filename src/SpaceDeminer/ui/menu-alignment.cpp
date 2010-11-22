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

#include "./menu-alignment.hpp"
#include <gdkmm.h>
#include <iostream>

namespace ui_frame_metrics
{
  extern Real t_w_x;
  extern Real t_w_y;
  extern Real t_w_w;
  extern Real t_w_h;

  extern Real t_e_w;
  extern Real t_e_h;
  extern Real t_e_x;
  extern Real t_e_y;
}

MenuAlignment::MenuAlignment()
{
  set(.5f, .5f, .0f, .0f);

  add(split_areas);
    split_areas.set_name("MenuAlignment::split_areas");
    split_areas.set_size_request(320, 320);
    split_areas.show();
    split_areas.set_spacing(0.f);
    split_areas.pack_start(title_area, false, false);
      title_area.show();
      title_area.set_size_request(64, 64);
      title_area.pack_start(page_icon, false, false);
        page_icon.set_size_request(54, 48);
        page_icon.set_alignment(0.5f, 0.0f);
        page_icon.show();
      title_area.pack_start(link_area);
        link_area.show();
        link_area.set(0.5, 0.5, 1., 1.);
        link_area.set_padding(
          4.f, // top
          4.f, // bottom
          (ui_frame_metrics::t_w_w-54.f)*.33f,//left
          ui_frame_metrics::t_e_w);//right
    split_areas.pack_start(main_area_sw, true, true);
      main_area_sw.set_name("MenuAlignment::main_area_sw");
      main_area_sw.show();
      main_area_sw.set_policy(Framework::POLICY_AUTOMATIC, Framework::POLICY_AUTOMATIC);
      main_area_sw.add(main_area);
        main_area.show();
}

MenuAlignment::~MenuAlignment()throw()
{
}

void MenuAlignment::on_size_allocate()
{
  Framework::Alignment::on_size_allocate();

  menu_back.resolution_changed();
}

void MenuAlignment::on_expose(Framework::Widget::EventExpose& event)
{
  if(get_child())
  {
    const Framework::Widget::Allocation& allocation  = get_child()->get_allocation();
/*
    if(event.get_invalid_area().get_x() < allocation.get_x() || event.get_invalid_area().get_y() < allocation.get_y() ||
       event.get_invalid_area().get_x()+event.get_invalid_area().get_width() > allocation.get_x()+allocation.get_width() ||
       event.get_invalid_area().get_y()+event.get_invalid_area().get_height() > allocation.get_y()+allocation.get_height())*/
    {
      //std::cout<<"event.area ( "<<event.area.x<<", "<<event.area.y<<", "<<event.area.width<<", "<<event.area.height<<" )\n";
      //std::cout<<"allocation ("<<allocation.get_x()<<", "<<allocation.get_y()<<", "<<allocation.get_width()<<", "<<allocation.get_height()<<")\n\n";

      menu_back.expose(event, get_allocation());
    }

    menu_frame.expose_background(event, allocation);

    Framework::Alignment::on_expose(event);

    menu_frame.expose(event, allocation);
  }else
  {
    menu_back.expose(event, get_allocation());

    Framework::Alignment::on_expose(event);
  }
}
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

#include "./../framework.hpp"

namespace Framework
{
  void DummyTheme::draw(PaintTool& ee, const Glib::ustring& what, DrawPass pass, guint32 param, const Gdk::Rectangle& where)const
  {
    int x = where.get_x();
    int y = where.get_y();
    int w = where.get_width();
    int h = where.get_height();

    gdouble front_color_r = 0.6;
    gdouble front_color_g = 0.6;
    gdouble front_color_b = 0.6;
    gdouble front_color_a = 1.;
    gdouble back_color_r = 0.2;
    gdouble back_color_g = 0.2;
    gdouble back_color_b = 0.2;
    gdouble back_color_a = 1.0;
    gdouble shadow_color_r = 0.0;
    gdouble shadow_color_g = 0.0;
    gdouble shadow_color_b = 0.0;
    gdouble shadow_color_a = 0.3;

    if(is_ment(what, "Button"))
    {
      if(pass==DRAWPASS_BACK)
      {
        int offset=0;

        switch(param)
        {
        case Framework::Button::DRAW_PARAM_RELIEF_PUSHED:
          offset  = 1;
        case Framework::Button::DRAW_PARAM_RELIEF_NORMAL:
        default:
          ee.draw_color_rect(x+2+offset, y+2+offset, w-4, h-4, back_color_r, back_color_r, back_color_r, shadow_color_a);
          ee.draw_rect_border(x+1+offset, y+1+offset, w-2, h-2, front_color_r, front_color_g, front_color_b, front_color_a*0.25, 1);
          ee.draw_rect_border(x+offset, y+offset, w, h, shadow_color_r, shadow_color_g, shadow_color_b, shadow_color_a, 1);
        }
      }
    }
  }

  void DummyTheme::get_metrics(const Glib::ustring& what, Metrics& m)const
  {
    m.set(0);
    if(is_ment(what, "child-dist/Button"))
    {
      m.set(3);
    }else if(is_ment(what, "child-dist/SHADOW_IN"))
    {
      m.set(1);
    }
  }

  ResPtr<Font> DummyTheme::create_font(const Glib::ustring& what)const
  {
    return default_font;
  }

  DummyTheme::DummyTheme()
  {
    default_font  = Font::create("sans", 12, false, false, true, 0.6f, 0.6f, 0.6f);
  }

  DummyTheme::~DummyTheme()throw()
  {
  }

  void DummyTheme::on_init()
  {
  }

  void DummyTheme::on_deinit()
  {
  }
}